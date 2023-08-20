#include "../include/Server.hpp"

Server::Server(const ServerConfig& config)
{	
	setHost(config.getConfigPairs()[HOST]);
	setPort(config.getConfigPairs()[PORT]);
	_configs = config.getAltConfigs();
	_configs.insert(_configs.begin(), config);
	applyHostConfig(config);	
}

void Server::applyHostConfig(const ServerConfig& config)
{
	strMap configPairs = config.getConfigPairs();
	
	// Set main values stored in configPairs
	setClientMaxBody(configPairs[CLIMAXBODY]);
	setMaxConnections(configPairs[MAXCONNS]); // not being used for now
	setDefaultDirListing(configPairs[DIRLISTING]);
	
	// Copy remaining values directly to server variables 
	_root = configPairs[ROOT];
	_standardFile = configPairs[STDFILE];
	_names = config.getNames();
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

bool Server::receiveData()
{
	if (!(_pollStruct->revents & POLLIN))
		return false;
	ANNOUNCEME_FD
	if (_clientIt->requestBodyComplete)
	{
		std::cout << "receiveData returning because reqeustbodyComplete" << std::endl;
		return true;
	}
	char buffer[RECV_CHUNK_SIZE];
	int bytesReceived = recv(_clientIt->fd, buffer, RECV_CHUNK_SIZE, 0);
	if (bytesReceived <= 0)
	{
		closeClient("Server::receiveData: 0 bytes received");
		throw std::runtime_error(I_CLOSENODATA);
	}
	_clientIt->buffer.append(buffer, bytesReceived);
	return true;
}

bool Server::sendData()
{
	if (!(_pollStruct->revents & POLLOUT))
		return false;
	if (!_clientIt->requestHeadComplete)
	{
		closeClient("Server::handleConnections: POLLOUT but no request Head");
		return false;
	}
	if (!_clientIt->requestFinished)
		return false;
	return true;
}

pollfd* Server::getPollStruct(int fd)
{
	std::vector<pollfd>::iterator it = _pollVector->begin();
		
	while (it != _pollVector->end() && it->fd != fd)
		++it;
	if (it == _pollVector->end())
		throw std::runtime_error("Server::handleConnections: fd to handle not found in pollVector");
	return &*it;
}

bool Server::hangUp()
{
	if (_pollStruct->revents & POLLHUP)
	{
		closeClient("Server::handleConnections: POLLHUP");
		return true;
	}
	return false;
}

bool Server::errorPending()
{
	if (!_clientIt->errorPending)
		return false;
	ANNOUNCEME_FD
	if (_pollStruct->revents & POLLOUT)
	{
		sendResponseHead();
		sendResponseBody();
	}
	return true;
}

void Server::handleConnections()
{
	for (_index = 0; _index < _clients.size(); ++_index)
	{
		_clientIt = _clients.begin() + _index;
		_pollStruct = getPollStruct(_clientIt->fd);
		
		ANNOUNCEME_FD
		try
		{
			if (hangUp())
				continue;
			if (errorPending())
				continue;
			if (receiveData() && requestHead())
			{
				if (_clientIt->method == GET)
					handleGet();
				else if (_clientIt->method == POST)
					handlePost();
				else if (_clientIt->method == DELETE)
					handleDelete();
			}
			if (sendData())
			{
				sendResponseHead();
				sendResponseBody();
			}
		}
		catch (const std::exception& e)
		{
			std::cerr << "handleConnections catch: " << e.what() << std::endl;
		}
	}
}

bool Server::requestError()
{
	// wrong protocol
	if (_clientIt->httpProtocol != HTTPVERSION)
		return (selectStatusPage(505), true);
	
	// method not supported by server
	if (_clientIt->method != GET
		&& _clientIt->method != POST
		&& _clientIt->method != DELETE)
		return (selectStatusPage(501), true);
	
	// body size too large
	if (_clientIt->contentLength > (int)_clientMaxBody)
		return (selectStatusPage(413), true);
	else
	{
		strLocMap_it locIt = _locations.find(_clientIt->directory);
		
		// access forbidden (have to specifically allow each path in config file)
		if (locIt == _locations.end())
			return (selectStatusPage(404), true); // returning 404, not 403, to not leak file structure
	
		// access granted, but not for the requested method
		else if ((_clientIt->method == GET && !locIt->second.get)
			|| (_clientIt->method == POST && !locIt->second.post)
			|| (_clientIt->method == DELETE && !locIt->second.delete_))
			return (selectStatusPage(405), true);
	}
	return false;
}

void Server::updateClientPath()
{
	_clientIt->dirListing = dirListing(_clientIt->directory);
	_clientIt->standardFile = _locations[_clientIt->directory].std_file;
	if (_clientIt->standardFile.empty())
		_clientIt->standardFile = _standardFile;
	std::string	http_redir = _locations[_clientIt->directory].http_redir;
	if (!http_redir.empty())
		_clientIt->directory = http_redir;
	if (_clientIt->method == POST)
		_clientIt->directory += _locations[_clientIt->directory].upload_dir;
	_clientIt->directory = prependRoot(_clientIt->directory);
	_clientIt->path = _clientIt->directory + _clientIt->filename;
}

bool Server::requestHead()
{
	if (_clientIt->requestHeadComplete)
		return true;
	ANNOUNCEME_FD
	if (_clientIt->buffer.find("\r\n\r\n") == std::string::npos)
	{
		selectStatusPage(431);
		return false;
	}
	_clientIt->parseRequest();
	if (requestError())
		return false;
	selectHostConfig();
	updateClientPath();
	_clientIt->whoIsI();
	return true;
}

void Server::selectHostConfig()
{
	if (_clientIt->host.empty())
	{
		applyHostConfig(_configs[0]);
		return;
	}
	for (size_t i = 0; i < _configs.size(); ++i)
	{
		if (stringInVec(_clientIt->host, _configs[i].getNames()))
		{
			applyHostConfig(_configs[i]);
			std::cout << "Hostname '" << _clientIt->host << "' found in ServerConfig #" << i << std::endl;
			return;
		}
	}
	std::cout << "Hostname '" << _clientIt->host << "' not found. Running default ServerConfig." << std::endl;
	applyHostConfig(_configs[0]);
}

void Server::handleDelete()
{
	if (isDirectory(_clientIt->path)) // deleting directories not allowed
	{
		selectStatusPage(405);
		return;
	}
	if (!resourceExists(_clientIt->path))
	{
		selectStatusPage(404);
		return;
	}
	if (remove(_clientIt->path.c_str()) == 0)
	{
		_clientIt->statusCode = 204;
		_clientIt->requestFinished = true;
	}
	else
		selectStatusPage(500);
}

void Server::handleGet()
{
	if (isDirectory(_clientIt->path))
	{
		if (resourceExists(_clientIt->path + _clientIt->standardFile))
		{
			_clientIt->statusCode = 200;
			_clientIt->sendPath = _clientIt->path + _clientIt->standardFile;
		}
		else if (_clientIt->dirListing)
		{
			_clientIt->statusCode = 200;
			// call dir listing
			_clientIt->sendPath = "system/dirListing.html";
		}
		else
			selectStatusPage(404);
	}
	else
	{
		if (resourceExists(_clientIt->path))
		{
			_clientIt->statusCode = 200;
			_clientIt->sendPath = _clientIt->path;
		}
		else
			selectStatusPage(404);
	}
	_clientIt->requestFinished = true;
}

void Server::handlePost()
{
	if (_clientIt->requestBodyComplete)
		return;
	if (!resourceExists(_clientIt->directory))
	{
		selectStatusPage(500);
		return;		
	}
	std::ofstream outputFile;
	if (_clientIt->append)
		outputFile.open(_clientIt->path.c_str(), std::ios::binary | std::ios::app);
	else
	{
		outputFile.open(_clientIt->path.c_str(), std::ios::binary | std::ios::trunc);
		_clientIt->append = true;
	}
	if (!outputFile.is_open())
	{
		selectStatusPage(500);
		return;		
	}
	outputFile.write(_clientIt->buffer.c_str(), _clientIt->buffer.size());
	_clientIt->bytesWritten += _clientIt->buffer.size();
	outputFile.close();
	if (_clientIt->bytesWritten >= (size_t)_clientIt->contentLength)
	{
		_clientIt->requestBodyComplete = true;
		_clientIt->requestFinished = true;
		_clientIt->statusCode = 201;
	}
}

std::string Server::buildResponseHead()
{
	std::stringstream ss_header;
	size_t contentLength = fileSize(_clientIt->sendPath);
	
	ss_header << HTTPVERSION << ' ' << _clientIt->statusCode << ' ' << getHttpMsg(_clientIt->statusCode) << "\r\n";
	ss_header << "Server: " << SERVERVERSION << "\r\n";
	if (contentLength != 0)
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
	ANNOUNCEME_FD

	std::string header = buildResponseHead();
	if (::send(_clientIt->fd, header.c_str(), header.size(), 0) == -1)
	{
		closeClient("Server::sendResponseHead: send failure.");
		throw std::runtime_error(E_SEND);
	}
	std::cout << "responseHead sent to fd: " << _clientIt->fd << "\n" << header << std::endl;
	_clientIt->responseHeadSent = true;
}

void Server::sendResponseBody()
{
	if (!_clientIt->responseHeadSent)
		return;
	ANNOUNCEME_FD
	std::cout << "sendPath:'" << _clientIt->sendPath << "'" << std::endl;
	
	if (_clientIt->sendPath.empty())
	{
		closeClient("Server::sendResponseBody: nothing to send.");
		return;
	}
	std::ifstream fileStream(_clientIt->sendPath.c_str(), std::ios::binary);
	if (fileStream.fail())
	{
		fileStream.close();
		closeClient("Server::sendResponseBody: ifstream failure.");
		throw std::runtime_error("sendResponseBody: Could not open file to send. Client closed.");
	}
	
	char buffer[SEND_CHUNK_SIZE];
	fileStream.seekg(_clientIt->filePosition);
	fileStream.read(buffer, SEND_CHUNK_SIZE);

	if (::send(_clientIt->fd, buffer, fileStream.gcount(), 0) == -1)
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
	if (msg)
		std::cout << "closeClient on fd " << _clientIt->fd << ": " << msg << std::endl;
	close(_clientIt->fd);
	
	// erase corresponding pollStruct
	std::vector<pollfd>::iterator it = _pollVector->begin();
	while (it != _pollVector->end() && it->fd != _clientIt->fd)
		++it;
	if (it == _pollVector->end())
		throw std::runtime_error("Server::closeClient: fd to close not found in pollVector");
	_pollVector->erase(it);
	
	// erase client and decrement _index to not skip the next client in the for loop
	_clients.erase(_clientIt);
	--_index;
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

void Server::selectStatusPage(int code)
{
	_clientIt->requestHeadComplete = true;
	_clientIt->requestBodyComplete = true;
	_clientIt->requestFinished = true;
	_clientIt->errorPending = true;
	_clientIt->statusCode = code;

	if (_errorPagesPaths.find(code) == _errorPagesPaths.end())
	{
		generateStatusPage(code);
		return;
	}
	std::string path = prependRoot(_errorPagesPaths[code]);
	if (resourceExists(path))
		_clientIt->sendPath = path;
	else
		generateStatusPage(code);
}

std::string Server::prependRoot(const std::string& path)
{
	if (path.find('/') == 0)
		return _root + path.substr(1);
	else
		return path;
}

void Server::generateStatusPage(int code)
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
