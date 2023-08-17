#include "../include/Server.hpp"

Server::Server(const ServerConfig & config)
{	
	// Set main values stored in configPairs
	strMap configPairs = config.getConfigPairs();
	setNames(configPairs[SERVERNAME]);
	setHost(configPairs[HOST]);
	setPort(configPairs[PORT]);
	setClientMaxBody(configPairs[CLIMAXBODY]);
	setMaxConnections(configPairs[MAXCONNS]);
	setDefaultDirListing(configPairs[DIRLISTING]);
	
	// Copy remaining values directly to server variables 
	_root = configPairs[ROOT];
	_errorPagesPaths = config.getErrorPaths();
	_locations = config.getLocations();
	_cgiPaths = config.getCgiPaths();
	_mimeTypes = config.getMIMETypes();
	_sharedNetAddr = config.getSharedNetAddr();
}

Server::~Server()
{
	while (!_clients.empty())
	{
		_clientIt = _clients.begin();
		closeClient("Server::~Server(): Server Object shutting down");
	}
	if (_server_fd != -1)
		close(_server_fd);
}

void	Server::startListening(std::vector<pollfd>& pollVector)
{
	ANNOUNCEME
	int		options = 1;
	pollfd	newPollStruct;
	
	_server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_server_fd == -1)
		throw std::runtime_error(E_SOCKET);
	if (setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&options, sizeof(options)) == -1)
		throw std::runtime_error(E_SOCKOPT);
	if (fcntl(_server_fd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error(E_FCNTL);
	if (bind(_server_fd, (struct sockaddr *) &_serverAddress, sizeof(_serverAddress)) == -1)
	{
		close(_server_fd);
		throw std::runtime_error(E_BIND);
	}
	if (listen(_server_fd, SOMAXCONN) == -1)
	{
		close(_server_fd);
		throw std::runtime_error(E_LISTEN);
	}
	newPollStruct.fd = _server_fd;
	newPollStruct.events = POLLIN;
	newPollStruct.revents = 0;
	pollVector.push_back(newPollStruct);
	_pollVector = &pollVector;
}

void Server::receiveData()
{
	ANNOUNCEME_FD
	if (_clientIt->requestBodyComplete)
	{
		std::cout << "receiveData returning because reqeustbodyComplete" << std::endl;
		return;
	}
	char buffer[RECV_CHUNK_SIZE];
	bzero(buffer, RECV_CHUNK_SIZE);
	int bytesReceived = recv(_clientfd, buffer, RECV_CHUNK_SIZE, 0);
	std::cout << bytesReceived << " bytes received.\nContent:\n" << buffer << std::endl;
	if (bytesReceived <= 0)
	{
		closeClient("Server::receiveData: 0 bytes received");
		throw std::runtime_error(I_CLOSENODATA);
	}
	_clientIt->buffer.append(buffer, bytesReceived);
}

pollfd* Server::getPollStruct(int fd)
{
	std::vector<pollfd>::iterator	it = _pollVector->begin();
		
	while (it != _pollVector->end() && it->fd != fd)
		++it;
	if (it == _pollVector->end())
		throw std::runtime_error("Server::handleConnections: fd to handle not found in pollVector");
	return &*it;
}

void Server::handleConnections()
{
	for (_clientIt = _clients.begin(); _clientIt != _clients.end(); ++_clientIt)
	{
		_clientfd = _clientIt->fd;
		pollfd*	pollStruct = getPollStruct(_clientfd);
		
		ANNOUNCEME_FD
		try
		{
			if (pollStruct->revents & POLLHUP)
			{
				closeClient("Server::handleConnections: POLLHUP");
				continue;
			}
			if (_clientIt->errorPending)
			{
				std::cout << "Error Pending." << std::endl;
				if (pollStruct->revents & POLLOUT)
				{
					sendResponseHead();
					sendResponseBody();
				}
				continue;
			}
			if (pollStruct->revents & POLLIN)
			{
				std::cout << "POLLIN." << std::endl;
				receiveData();
				handleRequestHead();
				handleRequestBody();
				selectResponseContent();
			}
			if (pollStruct->revents & POLLOUT)
			{
				std::cout << "POLLOUT." << std::endl;
				if (!_clientIt->requestHeadComplete)
				{
					closeClient("Server::handleConnections: POLLOUT but no request Head");
					continue;
				}
				sendResponseHead();
				sendResponseBody();
			}
		}
		catch (const std::exception& e)
		{
			std::cerr << "sendblock catch: " << e.what() << std::endl;
			
		}
		if (_clientIt == _clients.end())
			break;
	}
}

void Server::checkRequest()
{
	// wrong protocol
	if (_clientIt->httpProtocol != HTTPVERSION)
		selectErrorPage(505);
	
	// method not supported by server
	else if (_clientIt->method != GET
		&& _clientIt->method != POST
		&& _clientIt->method != DELETE)
		selectErrorPage(501);
	
	// body size too large
	else if (_clientIt->contentLength > (int)_clientMaxBody)
		selectErrorPage(413);
	else
	{
		strLocMap_it	locIt = _locations.find(_clientIt->directory);
		
		// access forbidden (have to specifically allow each path in config file)
		if (locIt == _locations.end())
			selectErrorPage(404); //  returning 404 (and not 403) to not leak file structure
	
		// access granted, but not for the requested method
		else if ((_clientIt->method == GET && !locIt->second.get)
			|| (_clientIt->method == POST && !locIt->second.post)
			|| (_clientIt->method == DELETE && !locIt->second.delete_))
			selectErrorPage(405);
	}
}

void Server:: handleRequestHead()
{
	if (_clientIt->requestHeadComplete)
		return;
	ANNOUNCEME_FD
	if (_clientIt->buffer.find("\r\n\r\n") == std::string::npos)
	{
		selectErrorPage(431);
		return;
	}
	_clientIt->parseRequest();
	_clientIt->requestHeadComplete = true;
	std::cout << "request filename: " << _clientIt->filename << std::endl;
	std::cout << "request method: " << _clientIt->method << std::endl;
	std::cout << "request path raw:'" << _clientIt->path << "'" << std::endl;
	std::cout << "completePath:'" << _root + _clientIt->path << "'" << std::endl;
	std::cout << "requestbody complete: " << (_clientIt->requestBodyComplete ? "yes" : "no") << std::endl;
	std::cout << "request directory: " << _clientIt->directory << std::endl;
	if (isDirectory(_root + _clientIt->directory))
		std::cout << _root + _clientIt->directory << " is a directory." << std::endl;
	checkRequest();
}

void Server::handleRequestBody()
{
	if (_clientIt->requestBodyComplete)
		return;
	ANNOUNCEME_FD
	std::string uploadDir = _root + _clientIt->directory + _locations[_clientIt->directory].upload_dir;
	std::cout << "extra upload_dir in locs: " << _locations[_clientIt->directory].upload_dir << std::endl;
	std::cout << "uploadDir: " << uploadDir << std::endl;
	std::cout << "buffer to write:\n" << _clientIt->buffer << std::endl;
	if (!resourceExists(uploadDir))
	{
		selectErrorPage(500);
		return;		
	}
	std::string writePath = uploadDir + _clientIt->filename;
	std::cout << "writePath:'" << writePath << "'" << std::endl;
	std::ofstream outputFile;
	
	if (_clientIt->append)
		outputFile.open(writePath.c_str(), std::ios::binary | std::ios::app);
	else
	{
		outputFile.open(writePath.c_str(), std::ios::binary | std::ios::trunc);
		_clientIt->append = true;
	}
	if (!outputFile.is_open())
		throw std::runtime_error(E_REQUESTFILE);
	outputFile.write(_clientIt->buffer.c_str(), _clientIt->buffer.size());
	_clientIt->bytesWritten += _clientIt->buffer.size();
	outputFile.close();
	if (_clientIt->bytesWritten >= (size_t)_clientIt->contentLength)
	{
		_clientIt->requestBodyComplete = true;
		selectResponseContent();
	}
}

std::string Server::buildResponseHead()
{
	std::stringstream	ss_header;
	
	size_t contentLength = 0;
	if (!_clientIt->sendPath.empty())
		contentLength = fileSize(_clientIt->sendPath);
	
	ss_header << HTTPVERSION << ' ' << _clientIt->statusCode << ' ' << getHttpMsg(_clientIt->statusCode) << "\r\n";
	ss_header << "Server: " << SERVERVERSION << "\r\n";
	ss_header << "content-type: " << mimeType(_clientIt->sendPath) << "\r\n";
	ss_header << "content-length: " << contentLength << "\r\n";
	ss_header << "connection: close" << "\r\n";
	ss_header << "\r\n";
	
	return ss_header.str();
}

void Server::sendResponseHead()
{
	if (_clientIt->responseHeadSent)
		return;
	if (!_clientIt->requestBodyComplete)
		return;
	if (!_clientIt->requestHeadComplete)
	{
		closeClient("Server::sendResponseHead: !requestHeadComplete");
		return;
	}
	ANNOUNCEME_FD

	std::string header = buildResponseHead();
	
	if (::send(_clientfd, header.c_str(), header.size(), 0) == -1)
		throw std::runtime_error(E_SEND);
	std::cout << "responseHead sent to fd: " << _clientfd << "\n" << header << std::endl;
	_clientIt->responseHeadSent = true;
}

void Server::selectResponseContent()
{
	if (_clientIt->responseFileSelected || !_clientIt->requestBodyComplete)
		return;
	ANNOUNCEME_FD
	// errors on post or delete already handled, so just have to set appropriate return code.
	// by not setting sendPath it remains empty and no body will be sent later.
	if (_clientIt->method == POST)
	{
		_clientIt->responseFileSelected = true;
		_clientIt->statusCode = 201;
		return;
	}
	if (_clientIt->method == DELETE)
	{
		_clientIt->responseFileSelected = true;
		_clientIt->statusCode = 204;
		return;
	}
	// only remaining method is GET
	// check for http redirection
	std::string	completePath = _root + _clientIt->path;
	if (!_locations[_clientIt->directory].http_redir.empty())
		completePath = _root + _locations[_clientIt->directory].http_redir + _clientIt->filename;
	std::cout << "completePath after HTTP redirection check: " << completePath << std::endl;
	if (isDirectory(completePath))
	{
		std::cout << "completePath is a directory" << std::endl;
		std::string standardFile = _locations[_clientIt->directory].std_file;
		if (standardFile.empty())
			standardFile = "index.html";
		if (resourceExists(completePath + standardFile))
		{
			std::cout << "'" << completePath + standardFile << "' exists." << std::endl;
			_clientIt->statusCode = 200;
			_clientIt->sendPath = completePath + standardFile;
		}
		else if (dirListing(completePath))
		{
			std::cout << "no, '" << completePath + standardFile << "' does not exist, but dir listing is allowed. Show dir listing here" << std::endl;
			_clientIt->statusCode = 200;
			_clientIt->sendPath = "dirlisting"; // dunno how to handle this best yet. wait to implemnt dirlisting to decide
		}
		else
		{
			std::cout << "'" << completePath + standardFile << "' does not exist, and dir listing is not allowed. Send 404." << std::endl;
			selectErrorPage(404);
		}
	}
	else // is not directory
	{
		std::cout << "completePath is not a directory" << std::endl;

		if (resourceExists(completePath))
		{
			std::cout << "resource exists" << std::endl;
			
			_clientIt->statusCode = 200;
			_clientIt->sendPath = completePath;
		}
		else
		{
			std::cout << "resource does not exist" << std::endl;
			selectErrorPage(404);
		}
	}
	_clientIt->responseFileSelected = true;
}

void Server::sendResponseBody()
{
	if (!_clientIt->requestHeadComplete || !_clientIt->responseFileSelected || !_clientIt->responseHeadSent)
		return;
	ANNOUNCEME_FD
	std::cout << "sendPath:'" << _clientIt->sendPath << "'" << std::endl;
	
	if (_clientIt->sendPath.empty())
	{
		closeClient("Server::sendResponseBody: sendPath empty.");
		return;
	}
	std::ifstream fileStream(_clientIt->sendPath.c_str(), std::ios::binary);
	if (fileStream.fail())
	{
		fileStream.close();
		closeClient("Server::sendResponseBody: ifstream failure.");
		throw std::runtime_error("sendResponseBody: Could not open file to send. Client closed.");
	}
	std::cout << "fileposition: " << _clientIt->filePosition << std::endl;
	fileStream.seekg(_clientIt->filePosition);
	char	buffer[SEND_CHUNK_SIZE];
	bzero(buffer, SEND_CHUNK_SIZE);
	fileStream.read(buffer, SEND_CHUNK_SIZE);

	if (::send(_clientfd, buffer, fileStream.gcount(), 0) == -1)
	{
		fileStream.close();
		closeClient("Server::sendResponseBody: send failure.");
		throw std::runtime_error(E_SEND);
	}
	if (fileStream.eof())
	{
		fileStream.close();
		closeClient("Server::sendResponseBody: sending complete.");
		return;
	}
	_clientIt->filePosition = fileStream.tellg();
	fileStream.close();
	std::cout << "sendFile.gcount to fd " << _clientfd << ": " << fileStream.gcount() << std::endl;
	
}	 
	
bool Server::dirListing(std::string path)
{
	strLocMap_it	locIt =_locations.find(path);
	
	if (locIt == _locations.end())
		return false;
	if (locIt->second.dir_listing == "yes")
		return true;
	if (locIt->second.dir_listing == "no")
		return false;
	if (!_defaultDirListing)
		return false;
	return true;
}

void Server::closeClient(const char* msg)
{
	std::vector<pollfd>::iterator it = _pollVector->begin();
	if (msg)
		std::cout << "closeClient fd " << _clientIt->fd << ": " << msg << std::endl;
	close(_clientfd);
	while (it != _pollVector->end() && it->fd != _clientfd)
		++it;
	if (it == _pollVector->end())
		throw std::runtime_error("Server::closeClient: fd to close not found in pollVector");
	_pollVector->erase(it);
	size_t clientIndex = std::distance(_clients.begin(), _clientIt);
	_clients.erase(_clientIt);
	_clientIt = _clients.begin() + clientIndex;
	if (_clientIt == _clients.end())
		throw std::runtime_error("Server::closeClient: new clientIt points to end after closeClient");
	_clientfd = _clientIt->fd;
}

void Server::whoIsI()
{
	std::cout	<< '\n'
				<< "Name(s):\t" << *_names.begin() << '\n';
					for (strVec_it it = ++_names.begin(); it != _names.end(); ++it)
						std::cout << "\t\t" << *it << '\n';
	std::cout	<< "Host:\t\t" << inet_ntoa(_serverAddress.sin_addr) << '\n'
				<< "Port:\t\t" << ntohs(_serverAddress.sin_port) << '\n'
				<< "Root:\t\t" << _root << '\n'
				<< "Dflt. dir_list:\t" << (_defaultDirListing ? "yes" : "no") << '\n'
				<< "Cl. max body:\t" << _clientMaxBody << '\n'
				<< "Max Conns:\t" << _maxConns << '\n'
				<< "Error Pages:\t" << _errorPagesPaths.begin()->first << '\t' << _errorPagesPaths.begin()->second << '\n';
					for (intStrMap_it it = ++_errorPagesPaths.begin(); it != _errorPagesPaths.end(); it++)
						std::cout << "\t\t" << it->first << '\t' << it->second << std::endl;
	std::cout	<< "Known loctns:\t" << _locations.begin()->first << '\n';
					for (strLocMap_it it = ++_locations.begin(); it != _locations.end(); it++)
						std::cout << "\t\t" << it->first << '\n';
	std::cout	<< "CGI Paths:\t" << _cgiPaths.begin()->first << '\t' << _cgiPaths.begin()->second << '\n';
					for (strMap_it it = ++_cgiPaths.begin(); it != _cgiPaths.end(); it++)
						std::cout << "\t\t" << it->first << '\t' << it->second << std::endl;
	std::cout	<< "Shared netw addr: " << (_sharedNetAddr ? "yes" : "no") << std::endl;
}

void Server::setNames(std::string input)
{
	std::string name;

	while (!input.empty())
	{
		name = splitEraseTrimChars(input, WHITESPACE);
		for (std::string::const_iterator it = name.begin(); it != name.end(); it++)
			if (!isalnum(*it) && *it != '.' && *it != '_')
				throw std::runtime_error(E_SERVERNAME + name + '\n');
		_names.push_back(name);
	}
}

void Server::setHost(std::string input)
{	
	if (input == "ANY")
		_serverAddress.sin_addr.s_addr = INADDR_ANY;
	else
	{
		_serverAddress.sin_addr.s_addr = inet_addr(input.c_str());
		if (_serverAddress.sin_addr.s_addr == INADDR_NONE)
			throw std::runtime_error(E_HOSTADDRVAL + input + '\n');
	}
	_serverAddress.sin_family = AF_INET;
}

void Server::setPort(std::string input)
{
	if (input.find_first_not_of("0123456789") != std::string::npos)
		throw std::runtime_error(E_PORTINPUT + input + '\n');
	uint16_t temp = (uint16_t) atoi(input.c_str());
	if (temp > (uint16_t) 65534)
		throw std::runtime_error(E_PORTVAL + input + '\n');
	_serverAddress.sin_port = htons(temp);
}

void Server::setClientMaxBody(std::string input)
{
	if (input.find_first_not_of("0123456789") != std::string::npos)
			throw std::runtime_error(E_MAXCLIENTBODYINPUT + input + '\n');
	_clientMaxBody = atoi(input.c_str());
	if (_clientMaxBody > MAX_MAXCLIENTBODY)
		throw std::runtime_error(E_MAXCLIENTBODYVAL + input + '\n');
}

void Server::setMaxConnections(std::string input)
{
	if (input.find_first_not_of("0123456789") != std::string::npos)
			throw std::runtime_error(E_MAXCONNINPUT + input + '\n');
	_maxConns = atoi(input.c_str());
	if (_maxConns > MAX_MAXCONNECTIONS)
		throw std::runtime_error(E_MAXCONNVAL + input + '\n');
}

void Server::setDefaultDirListing(std::string input)
{
	if (input == "yes")
		_defaultDirListing = true;
	else
		_defaultDirListing = false;
}

std::string	Server::mimeType(std::string filepath)
{
	size_t		dotPosition;
	std::string extension;
	strMap_it	it;
	std::string defaultType = "application/octet-stream";

	dotPosition = filepath.find_last_of(".");
	if (dotPosition == std::string::npos)
		return defaultType;
	extension = filepath.substr(dotPosition);
	it = _mimeTypes->find(extension);
	if (it != _mimeTypes->end())
		return it->second;
	return defaultType;
}

void Server::selectErrorPage(int code)
{
	_clientIt->requestHeadComplete = true;
	_clientIt->requestBodyComplete = true;
	_clientIt->responseFileSelected = true;
	_clientIt->errorPending = true;
	_clientIt->statusCode = code;

	if (_errorPagesPaths.find(code) != _errorPagesPaths.end() && resourceExists(_root + _errorPagesPaths[code]))
		_clientIt->sendPath = _root + _errorPagesPaths[code];
	else
		generateErrorPage(code);
}

void Server::generateErrorPage(int code)
{
	std::ofstream errorPage("system/errorPage.html", std::ios::binary | std::ios::trunc);
	if (errorPage.fail())
	{
		errorPage.close();
		std::cerr << "Error opening temporary file." << std::endl;
		throw std::runtime_error(E_TEMPFILE);
	}

	std::stringstream	ss_body;
	std::string			httpMsg = getHttpMsg(code);

	ss_body << "<!DOCTYPE html>";
	ss_body << "<html>\n";
	ss_body << "<head>\n";
	ss_body << "<title>webserv - " << code << ": " << httpMsg << "</title>\n";
	ss_body << "<style>\n";
	ss_body << "body {background-color: black; color: white; font-family: Arial, sans-serif; margin: 0; padding: 5% 0 0 0; text-align: center;}\n";
	ss_body << "h1 {font-size: 42px;}\n";
	ss_body << "p {font-size: 16px; line-height: 1.5;}\n";
	ss_body << "</style>\n";
	ss_body << "</head>\n";
	ss_body << "<body>\n";
	ss_body << "<h1>" << code << ": " << httpMsg << "</h1>\n";
	ss_body << "<img style=\"margin-left: auto;\" src=\"https://http.cat/" << code << "\" alt=\"" << httpMsg << "\">\n";
	ss_body << "</body>\n";
	ss_body << "</html>\n";
	
	errorPage.write(ss_body.str().c_str(), ss_body.str().size());
	errorPage.close();
	_clientIt->sendPath = "system/errorPage.html";
}

int Server::fd()
{
	return _server_fd;
}

void Server::addClient(int fd)
{
	_clients.push_back(Client(fd));
}

const char *	Server::invalidAddressException::what() const throw()
{
	return ("invalid IP address supplied.");
}
const char *	Server::fileDescriptorControlFailureException::what() const throw()
{
	return ("error controlling file descriptor to non-blocking.");
}
const char *	Server::sendFailureException::what() const throw()
{
	return ("error sending data to client.");
}
