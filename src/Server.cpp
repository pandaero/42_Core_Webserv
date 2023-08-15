#include "../include/Server.hpp"

Server::Server(const ServerConfig & config)
{	
	// Set main values stored in configPairs
	strMap		configPairs = config.getConfigPairs();
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

void	Server::startListening()
{
	ANNOUNCEME
	int		options = 1;
	pollfd	serverPollStruct;
	
	_server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_server_fd == -1)
		throw socketCreationFailureException();
	
	if (setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&options, sizeof(options)) == -1)
		throw std::runtime_error(E_SOCKOPT);
	
	if (fcntl(_server_fd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error(E_FCNTL);

	if (bind(_server_fd, (struct sockaddr *) &_serverAddress, sizeof(_serverAddress)) == -1)
	{
		close(_server_fd);
		throw bindFailureException();
	}
	
	if (listen(_server_fd, SOMAXCONN) == -1)
	{
		close(_server_fd);
		throw listenFailureException();
	}
	serverPollStruct.fd = _server_fd;
	serverPollStruct.events = POLLIN;
	serverPollStruct.revents = 0;
	_pollVector.push_back(serverPollStruct);
}

void Server::poll()
{
	ANNOUNCEME
	if (::poll(&_pollVector[0], _pollVector.size(), -1) == -1)
		throw std::runtime_error(E_POLL);
}

void Server::acceptConnections()
{
	if (_pollVector[0].revents & POLLIN)
	{
		ANNOUNCEME
		while (true)
		{
			if (_pollVector.size() > _maxConns)
			{
				std::cerr << I_CONNECTIONLIMIT << std::endl;
				return;
			}

			int 	new_sock;
			pollfd	new_pollStruct;

			new_sock = accept(_server_fd, NULL, NULL); // don't need client info, so pass NULL
			if (new_sock == -1)
			{
				if (errno != EWOULDBLOCK)
					std::cerr << E_ACCEPT << std::endl;
				break;
			}
			_clients.push_back(Client(0)); //new this shit?
			_clients.back().fd = new_sock;
			
			new_pollStruct.fd = new_sock;
			new_pollStruct.events = POLLIN | POLLOUT | POLLHUP;
			new_pollStruct.revents = 0;
			_pollVector.push_back(new_pollStruct);
			std::cout << "New client accepted on fd " << new_sock << "." << std::endl;
			std::cout << "Clients connected: " << _clients.size() << std::endl;
			std::cout << "PollStructs in Vector: " << _pollVector.size() << std::endl;
		}
	}
}

void Server::receiveData()
{
	int		bytesReceived;
	
	ANNOUNCEME_FD
	if (_clientIt->requestBodyComplete)
	{
		std::cout << "receiveData returning because reqeustbodyComplete" << std::endl;
		return;
	}
	char buffer[RECV_CHUNK_SIZE];
	bzero(buffer, RECV_CHUNK_SIZE);
	bytesReceived = recv(_clientfd, buffer, RECV_CHUNK_SIZE, 0);
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
	std::vector<pollfd>::iterator	it = _pollVector.begin();
		
		while (it != _pollVector.end() && it->fd != fd)
			++it;
		if (it == _pollVector.end())
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
				return;
			}
			continue;
		}
		try
		{
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
	if (_clientIt->httpProtocol() != HTTPVERSION)
		selectErrorPage(505);
	
	// method not supported by server
	else if (_clientIt->method() != GET
		&& _clientIt->method() != POST
		&& _clientIt->method() != DELETE)
		selectErrorPage(501);
	
	// body size too large
	else if (_clientIt->contentLength() > (int)_clientMaxBody)
		selectErrorPage(413);
	else
	{
		strLocMap_it	locIt = _locations.find(_clientIt->directory);
		
		// access forbidden (have to specifically allow each path in config file)
		if (locIt == _locations.end())
			selectErrorPage(404); // only returning 404 (and not 403) to not leak file structure
	
		// access granted, but not for the requested method
		else if ((_clientIt->method() == GET && !locIt->second.get)
			|| (_clientIt->method() == POST && !locIt->second.post)
			|| (_clientIt->method() == DELETE && !locIt->second.delete_))
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
	_clientIt->buildRequest();
	_clientIt->requestHeadComplete = true;
	std::cout << "request method: " << _clientIt->method() << std::endl;
	std::cout << "request path raw:'" << _clientIt->path() << "'" << std::endl;
	std::cout << "completePath:'" << _root + _clientIt->path() << "'" << std::endl;
	checkRequest();
}

void Server::handleRequestBody()
{
	if (_clientIt->requestBodyComplete)
		return;
	if (_clientIt->contentLength() < 0)
	{
		std::cout << "shouldnt ever get in here" << std::endl;
		_clientIt->requestBodyComplete = true;
		return;
	}
	
	// also need to check if CGI or not?
	std::string writePath  = _root + _clientIt->path() + _locations[_clientIt->path()].upload_dir;
	if (!resourceExists(writePath))
	{
		selectErrorPage(500);
		return;		
	}
	std::ofstream outputFile(writePath.c_str(), std::ios::binary | std::ios::app);
	if (!outputFile.is_open())
		throw std::runtime_error(E_REQUESTFILE);
	outputFile.write(_clientIt->buffer.c_str(), _clientIt->buffer.size());
	_clientIt->bytesWritten += _clientIt->buffer.size(); // outputFile.tellp() delta prolly better but nah.
	outputFile.close();
	if (_clientIt->bytesWritten >= (size_t)_clientIt->contentLength())
	{
		_clientIt->requestBodyComplete = true;
		selectResponseContent();
	}
}

void Server::sendResponseHead()
{
	if (_clientIt->responseHeadSent)
		return;
	if (!_clientIt->requestBodyComplete)
	{
		closeClient("Server::sendResponseHead: !requestBodyComplete");
		throw std::runtime_error("Server::sendResponseHead: !requestBodyComplete");
	}
	ANNOUNCEME_FD

	size_t	contentLength;

	if (_clientIt->sendPath.empty())
		contentLength = 0;
	else
		contentLength = fileSize(_clientIt->sendPath);
	
	std::stringstream	ss_header;
	
	ss_header << HTTPVERSION << ' ' << _clientIt->statusCode << ' ' << getHttpMsg(_clientIt->statusCode) << "\r\n";
	ss_header << "Server: " << SERVERVERSION << "\r\n";
	ss_header << "content-type: " << mimeType(_clientIt->sendPath) << "\r\n";
	ss_header << "content-length: " << contentLength << "\r\n";
	ss_header << "connection: close" << "\r\n";
	ss_header << "\r\n";
	
	if (::send(_clientfd, ss_header.str().c_str(), ss_header.str().size(), 0) == -1)
		throw std::runtime_error(E_SEND);
	std::cout << "responseHead sent to fd: " << _clientfd << "\n" << ss_header.str() << std::endl;
	_clientIt->responseHeadSent = true;
}

void Server::selectResponseContent()
{
	std::string	completePath;
	
	if (_clientIt->responseFileSelected || !_clientIt->requestBodyComplete)
		return;
	ANNOUNCEME_FD
	if (_clientIt->method() == POST)
	{
		_clientIt->responseFileSelected = true;
		_clientIt->statusCode = 201; //errors on post would have already been handled, so only reamining case is ok
		//_clientIt->sendPath.clear(); //shoujlndt neede this, should be empty upon init
		return;
	}
	// check for http redirection
	if (_locations[_clientIt->directory].http_redir.empty())
		completePath = _root + _clientIt->path();
	else
		completePath = _root + _locations[_clientIt->directory].http_redir + _clientIt->filename;

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
	
/*
// CGI handling (for php and potentially python scripts)
			// if (request.path() == ".php")
			// {
			// 	pid = fork ()
			// 	if (pid == 0)
			// 	{
			// 		// run cgi with input file as argument or piped in
			// 		// save file to temp directory
			// 	}
			// 	// attempt to serve file (html from cgi)
			// }
*/

// change other functions also to take const string &
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
	std::vector<pollfd>::iterator	it = _pollVector.begin();
	size_t							clientIndex;
	
	if (msg)
		std::cout << "closeClient fd " << _clientIt->fd << ": " << msg << std::endl;
	close(_clientfd);
	while (it != _pollVector.end() && it->fd != _clientfd)
		++it;
	if (it == _pollVector.end())
		throw std::runtime_error("Server::closeClient: fd to close not found in pollVector");
	_pollVector.erase(it);
	clientIndex = std::distance(_clients.begin(), _clientIt);
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

/* void Server::setBacklog(std::string input)
{
	if (input.find_first_not_of("0123456789") != std::string::npos)
			throw std::runtime_error(E_BACKLOGINPUT + input + '\n');
	_backlog = atoi(input.c_str());
	if (_backlog > MAX_BACKLOG)
		throw std::runtime_error(E_BACKLOGVAL + input + '\n');
} */

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
	
	std::cout << "errorpage troubleshoot: " << _root + _errorPagesPaths[code] << std::endl;

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

	std::string			httpMsg(getHttpMsg(code));
	std::stringstream	ss_body;

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

/* void Server::checkMethodAccess(std::string path)
{
	if (_GET && access(path.c_str(), R_OK) != 0)
		throw std::runtime_error(E_ACC_READ + path + '\n');
	if ((_POST | _DELETE) && access(path.c_str(), W_OK) != 0)
		throw std::runtime_error(E_ACC_WRITE + path + '\n');
}

void Server::checkReadAccess(std::string path)
{
	if (access(path.c_str(), R_OK) != 0)
		throw std::runtime_error(E_ACC_READ + path + '\n');
}

void Server::checkWriteAccess(std::string path)
{
	if (access(path.c_str(), W_OK) != 0)
		throw std::runtime_error(E_ACC_WRITE + path + '\n');
}

void Server::checkExecAccess(std::string path)
{
	if (access(path.c_str(), X_OK) != 0)
		throw std::runtime_error(E_ACC_EXEC + path + '\n');
} */

const char *	Server::invalidAddressException::what() const throw()
{
	return ("invalid IP address supplied.");
}

const char *	Server::socketCreationFailureException::what() const throw()
{
	return ("error creating socket for server.");
}

const char *	Server::fileDescriptorControlFailureException::what() const throw()
{
	return ("error controlling file descriptor to non-blocking.");
}

const char *	Server::bindFailureException::what() const throw()
{
	return ("error using bind.");
}

const char *	Server::listenFailureException::what() const throw()
{
	return ("error using listen.");
}
const char *	Server::sendFailureException::what() const throw()
{
	return ("error sending data to client.");
}
