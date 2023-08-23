#include "../include/Server.hpp"
#include <errno.h>

Server::Server(const ServerConfig& config)
{	
	_server_fd = -1;
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

int Server::fd()
{
	return _server_fd;
}

Server::~Server()
{
	if (_server_fd != -1)
	{
		std::cout << "Server destructor on listening fd " << _server_fd << "." << std::endl;
		close(_server_fd);
	}
	while (!_clients.empty())
	{
		_clientIt = _clients.begin();
		closeClient("Server::~Server(): Server Object shutting down");
	}
}

void Server::whoIsI()
{
	std::cout	<< '\n';
				if (!_names.empty())
				{
					std::cout << "Name(s):\t" << *_names.begin() << '\n';
					for (strVec_it it = ++_names.begin(); it != _names.end(); ++it)
						std::cout << "\t\t" << *it << '\n';
				}
	std::cout	<< "Host:\t\t" << inet_ntoa(_serverAddress.sin_addr) << '\n'
				<< "Port:\t\t" << ntohs(_serverAddress.sin_port) << '\n'
				<< "Root:\t\t" << _root << '\n'
				<< "Dflt. dir_list:\t" << (_defaultDirListing ? "yes" : "no") << '\n'
				<< "Cl. max body:\t" << _clientMaxBody << '\n'
				<< "Max Conns:\t" << _maxConns << '\n'
				<< "standardfile:\t" << _standardFile << '\n';
				if (!_errorPagesPaths.empty())
				{
					std::cout << "Error Pages:\t" << _errorPagesPaths.begin()->first << '\t' << _errorPagesPaths.begin()->second << '\n';
					for (intStrMap_it it = ++_errorPagesPaths.begin(); it != _errorPagesPaths.end(); it++)
						std::cout << "\t\t" << it->first << '\t' << it->second << std::endl;
				}
				if (!_locations.empty())
				{
					std::cout	<< "Known loctns:\t" << _locations.begin()->first << '\n';
					for (strLocMap_it it = ++_locations.begin(); it != _locations.end(); it++)
						std::cout << "\t\t" << it->first << '\n';
				}
				if (!_cgiPaths.empty())
				{
					std::cout	<< "CGI Paths:\t" << _cgiPaths.begin()->first << '\t' << _cgiPaths.begin()->second << '\n';
					for (strMap_it it = ++_cgiPaths.begin(); it != _cgiPaths.end(); it++)
						std::cout << "\t\t" << it->first << '\t' << it->second << std::endl;
				}
}

void Server::startListening(std::vector<pollfd>& pollVector)
{
	ANNOUNCEME
	int		options = 1;
	pollfd	newPollStruct;
	
	_server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_server_fd == -1)
		closeAndThrow(_server_fd);
	if (setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&options, sizeof(options)) == -1)
		closeAndThrow(_server_fd);;
	if (fcntl(_server_fd, F_SETFL, O_NONBLOCK) == -1)
		closeAndThrow(_server_fd);
	if (bind(_server_fd, (struct sockaddr*) &_serverAddress, sizeof(_serverAddress)) == -1)
		closeAndThrow(_server_fd);
	if (listen(_server_fd, SOMAXCONN) == -1)
		closeAndThrow(_server_fd);
	
	newPollStruct.fd = _server_fd;
	newPollStruct.events = POLLIN;
	newPollStruct.revents = 0;
	pollVector.push_back(newPollStruct);
	_pollVector = &pollVector;
}

/*
currently not checking for a size restriction. Revisit this during testing.
prolly just returns from accept with -1. 
if (pollVector.size() > 420)
	{
		std::cerr << I_CONNECTIONLIMIT << std::endl;
		return;
	}
*/
void Server::acceptConnections()
{
	ANNOUNCEME
	
	if (!(getPollStruct(_server_fd)->revents & POLLIN))
		return;
	while (true)
	{
		int new_sock = accept(_server_fd, NULL, NULL); // don't need client info, so pass NULL
		if (new_sock == -1)
		{
			if (errno != EWOULDBLOCK)
				std::cerr << E_ACCEPT << std::endl;
			return;
		}
		int flags = fcntl(new_sock, F_GETFL, 0);
		if (fcntl(new_sock, F_SETFL, flags | O_NONBLOCK) == -1)
			closeAndThrow(new_sock);
		
		Client newClient;
		newClient.fd = new_sock;
		_clients.push_back(newClient);

		pollfd new_pollStruct;
		new_pollStruct.fd = new_sock;
		new_pollStruct.events = POLLIN | POLLOUT | POLLHUP;
		new_pollStruct.revents = 0;
		_pollVector->push_back(new_pollStruct);
	}
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
			if (requestHead())
			{
				if (_clientIt->method == GET)
					handleGet();
				else if (_clientIt->method == POST)
					handlePost();
				else if (_clientIt->method == DELETE)
					handleDelete();
			}
			if (sendData() && responseHead())
				sendResponseBody();
		}
		catch (const std::exception& e)
		{
			std::cerr << "handleConnections catch: " << e.what() << std::endl;
		}
	}
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

bool Server::receive()
{
	if (!(_pollStruct->revents & POLLIN))
		return false;
	ANNOUNCEME_FD
	char buffer[RECV_CHUNK_SIZE];
	int bytesReceived = recv(_clientIt->fd, buffer, RECV_CHUNK_SIZE, 0);
	if (bytesReceived <= 0)
	{
		closeClient("Server::receiveData: 0 bytes received or error");
		throw std::runtime_error(I_CLOSENODATA);
	}
	_clientIt->buffer.append(buffer, bytesReceived);
	return true;
}

bool Server::requestHead()
{
	if (_clientIt->state > recv_head) // done receiving request head
		return true;
	ANNOUNCEME_FD
	if (!receive())
		return false;
	if (_clientIt->buffer.find("\r\n\r\n") == std::string::npos)
	{
		selectStatusPage(431);
		return false;
	}
	_clientIt->parseRequest();
	generateCookieLogPage();
	if (requestError())
		return false;
	selectHostConfig();
	updateClientPath();
	_clientIt->whoIsI();
	return true;
}

void Server::handleGet()
{
	if (_clientIt->state > handleRequest)
		return;
	if (cgiRequest())
	{
		doTheCGI();
		return;
	}
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
			_clientIt->sendPath = createDirList(_clientIt->path);
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
	_clientIt-> state = send_head;
}

void Server::handlePost()
{
	if (_clientIt->state > handleRequest)
		return;
	ANNOUNCEME_FD
	if (!resourceExists(_clientIt->directory))
	{
		selectStatusPage(500);
		return;		
	}
	if (_clientIt->state == recv_body)
		receive();
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
	std::cout << "before writing test. bytesWritten:" << _clientIt->bytesWritten << std::endl;
	outputFile.write(_clientIt->buffer.c_str(), _clientIt->buffer.size());
	_clientIt->bytesWritten += _clientIt->buffer.size();
	_clientIt->buffer.clear();
	outputFile.close();
	std::cout << "before bytesWritten test. bytesWritten:" << _clientIt->bytesWritten << std::endl;

	if (_clientIt->bytesWritten >= _clientIt->contentLength)
	{
		std::cout << "bytesWritten >= size evald positive!" << std::endl;
		if (cgiRequest())
			doTheCGI();
		else
		{
			_clientIt->statusCode = 201;
			_clientIt->state = send_head;
		}
	}
}

void Server::handleDelete()
{
	if (_clientIt->state > handleRequest)
		return;
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
		_clientIt->state = send_head;
	}
	else
		selectStatusPage(500);
}

bool Server::sendData()
{
	if (!(_pollStruct->revents & POLLOUT))
		return false;
	
	// situation where no data was read from the socket, but POLLOUT is active
	// and no further data is being sent. Would cause and endless loop.
	// on mac: there actually is data in the socket buffer, but POLLIN is not triggered
	// recv call without reading triggers POLLIN for the next run of the loop.
	// on WSL2 Ubuntu this also happens, but there is no data to be read (recv returns -1). Just close client.
	// No idea what causes this, researched it extensively.
	// Seems to occur when one client opens 2 connections at once. There is no evidence in the network tab
	// of the client of these weird requests. 
	if (_clientIt->state == recv_head)
	{
		if (recv(_clientIt->fd, NULL, 0, MSG_PEEK) == 0)
		{
			std::cout << "Residual data on socket buffer without POLLIN. POLLIN triggered for next loop iteration." << std::endl;
			return false;
		}
		closeClient("Server::sendData: Nothing to read and no request head.");
		return false;
	}

	if (_clientIt->state == recv_body)
		return false;
	return true;
}

bool Server::responseHead()
{
	if (_clientIt->state > send_head)
		return true;
	ANNOUNCEME_FD

	std::string header = buildResponseHead();
	if (send(_clientIt->fd, header.c_str(), header.size(), 0) <= 0)
	{
		closeClient("Server::sendResponseHead: send failure.");
		throw std::runtime_error(E_SEND);
	}
	std::cout << "responseHead sent to fd: " << _clientIt->fd << "\n" << header << std::endl;
	_clientIt->state = send_body;
	return false;
}

void Server::sendResponseBody()
{
	if (_clientIt->state < send_body)
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

	if (send(_clientIt->fd, buffer, fileStream.gcount(), 0) <= 0)
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

std::string Server::buildResponseHead()
{
	std::stringstream ss_header;
	size_t contentLength = fileSize(_clientIt->sendPath);
	
	ss_header << HTTPVERSION << ' ' << _clientIt->statusCode << ' ' << getHttpMsg(_clientIt->statusCode) << "\r\n";
	ss_header << "Server: " << SERVERVERSION << "\r\n";
	if (contentLength != 0)
		ss_header << "content-type: " << mimeType(_clientIt->sendPath) << "\r\n";
	ss_header << "content-length: " << contentLength << "\r\n";
	if (_clientIt->setCookie)
		ss_header << makeCookie(SESSIONID, _clientIt->sessionId, 3600, "/") << "\r\n";
	ss_header << "connection: close" << "\r\n";
	ss_header << "\r\n";
	return ss_header.str();
}

void Server::updateClientPath()
{
	// set dir listing
	_clientIt->dirListing = dirListing(_clientIt->directory);
	
	// select the file to try to serve in case of directory
	_clientIt->standardFile = _locations[_clientIt->directory].std_file;
	if (_clientIt->standardFile.empty())
		_clientIt->standardFile = _standardFile;
	
	// check for HTTP redirection
	std::string	http_redir = _locations[_clientIt->directory].http_redir;
	if (!http_redir.empty())
		_clientIt->directory = http_redir;
	
	// check for upload redirection
	if (_clientIt->method == POST && !_locations[_clientIt->directory].upload_dir.empty())
		_clientIt->directory = _locations[_clientIt->directory].upload_dir;
	
	// prepend the server root if path begins with /
	_clientIt->directory = prependRoot(_clientIt->directory);

	// build the new request path
	_clientIt->path = _clientIt->directory + _clientIt->filename;
}

void Server::selectStatusPage(int code)
{
	_clientIt->statusCode = code;
	_clientIt->state = send_head;

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

void Server::generateStatusPage(int code)
{
	std::ofstream errorPage(SYS_ERRPAGE, std::ios::binary | std::ios::trunc);
	if (errorPage.fail())
	{
		errorPage.close();
		throw std::runtime_error(E_TEMPFILE);
	}

	std::string httpMsg = getHttpMsg(code);

	errorPage << "<!DOCTYPE html>";
	errorPage << "<html>\n";
	errorPage << "<head>\n";
	errorPage << "<title>webserv - " << code << ": " << httpMsg << "</title>\n";
	errorPage << "<style>\n";
	errorPage << "body {background-color: black; color: white; font-family: Arial, sans-serif; margin: 0; padding: 5% 0 0 0; text-align: center;}\n";
	errorPage << "h1 {font-size: 42px;}\n";
	errorPage << "p {font-size: 16px; line-height: 1.5;}\n";
	errorPage << "</style>\n";
	errorPage << "</head>\n";
	errorPage << "<body>\n";
	errorPage << "<h1>" << code << ": " << httpMsg << "</h1>\n";
	errorPage << "<img style=\"margin-left: auto;\" src=\"https://http.cat/" << code << "\" alt=\"" << httpMsg << "\">\n";
	errorPage << "</body>\n";
	errorPage << "</html>\n";
	errorPage.close();
	_clientIt->sendPath = SYS_ERRPAGE;
}

/*
A better way to handle this would be to not write the data from the ServerConfig to
the Server object, but to just point to the currently active ServerConfig and call
its getters.
*/
void Server::selectHostConfig()
{
	if (_clientIt->host.empty())
	{
		applyHostConfig(_configs[0]);
		_activeServerName = _configs[0].getNames()[0];
		return;
	}
	for (size_t i = 0; i < _configs.size(); ++i)
	{
		if (stringInVec(_clientIt->host, _configs[i].getNames()))
		{
			std::cout << "Hostname '" << _clientIt->host << "' found in ServerConfig #" << i << std::endl;
			applyHostConfig(_configs[i]);
			_activeServerName = _clientIt->host;
			return;
		}
	}
	std::cout << "Hostname '" << _clientIt->host << "' not found. Running default ServerConfig." << std::endl;
	applyHostConfig(_configs[0]);
	_activeServerName = _configs[0].getNames()[0];
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
	if (_clientIt->contentLength > _clientMaxBody)
		return (selectStatusPage(413), true);
	else
	{
		strLocMap_it locIt = _locations.find(_clientIt->directory);
		
		// access forbidden (have to specifically allow each path in config file)
		if (locIt == _locations.end())
			return (selectStatusPage(404), true); // returning 404, not 403, to not leak file structure
	
		// access granted, but not for the requested method
		if ((_clientIt->method == GET && !locIt->second.get)
			|| (_clientIt->method == POST && !locIt->second.post)
			|| (_clientIt->method == DELETE && !locIt->second.delete_))
			return (selectStatusPage(405), true);
	}
	return false;
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
	std::cout << "closeClient: erasing pollStruct with fd " << it->fd << std::endl;
	_pollVector->erase(it);
	
	// erase client and decrement _index to not skip the next client in the for loop
	_clients.erase(_clientIt);
	--_index;
}

bool Server::cgiRequest()
{
	std::string extension = fileExtension(_clientIt->filename);
	if (extension == ".py" || extension == ".php")
	{
		_cgiExtension = extension;
		return true;
	}
	return false;
}

void	Server::doTheCGI()
{
	std::ofstream	alilog("alilog.txt");
	alilog << "I got here!\n";

	std::string	pathToScript = "cgi-bin/" + _clientIt->filename;
	std::string pathToExec;
	alilog << "pathToScript: " << pathToScript << "\n";
	alilog << "_cgiExtension: " << _cgiExtension << "\n";

	if (_cgiExtension == ".py") 
		pathToExec = "/usr/bin/python3"; // find path to python3
	else if (_cgiExtension == ".php")
		pathToExec = "default/cgi/php-8.2.5_MacOS-10.15";
	alilog << "pathToExec: " << pathToExec << "\n";

	int		pipeFd[2];

	if (pipe(pipeFd) == -1)
		selectStatusPage(500);

	pid_t	pid = fork();

	if (pid == -1)
		selectStatusPage(500);

	if (pid == 0) {
		alilog << "made it to child process\n";
		close(pipeFd[0]);
		if (dup2(pipeFd[1], STDOUT_FILENO) == -1)
            selectStatusPage(500);
// env filler - it's only needed in child process
		std::vector<std::string>	tmpEnv;
		std::string	tmpVar;
		char	*_env[14];

	// env variables that are not request specific
		//this one can be anything bc we have our own webserv
		tmpVar = "SERVER_SOFTWARE=webservInC++98BabyLetsGo/4.2";
		tmpEnv.push_back(tmpVar);
		//server's hostname or IP address
		tmpVar = "SERVER_NAME=" + _activeServerName;
		tmpEnv.push_back(tmpVar);
		tmpVar = "GATEWAY_INTERFACE=CGI/1.2";
		tmpEnv.push_back(tmpVar);

	// env variables specific to the request
		tmpVar = "SERVER_PROTOCOL=HTTP/1.1";
		tmpEnv.push_back(tmpVar);
		//port number to which request was sent
		
		//tmpVar = "SERVER_PORT=" + ntohs(_serverAddress.sin_port);
		std::stringstream tempStream;
		tempStream << "SERVER_PORT=";
		tempStream << ntohs(_serverAddress.sin_port);
		tmpEnv.push_back(tempStream.str());
		
		tmpVar = "REQUEST_METHOD=" + _clientIt->method;
		tmpEnv.push_back(tmpVar);
		tmpVar = "PATH_INFO=" + pathToScript;
		tmpEnv.push_back(tmpVar);
		tmpVar = "SCRIPT_NAME=" + _clientIt->filename;
		tmpEnv.push_back(tmpVar);
		tmpVar = "QUERY_STRING=" + _clientIt->queryString;
		tmpEnv.push_back(tmpVar);
		alilog << "_clientIt->contentType: " << _clientIt->contentType << "\n";
		tmpVar = "CONTENT_TYPE=" + _clientIt->contentType;
		tmpEnv.push_back(tmpVar);
		
		//tmpVar = "CONTENT_LENGTH=" + _clientIt->contentLength; //maybe cast is needed
		tempStream.str("");
		tempStream.clear();
		tempStream << "CONTENT_LENGTH=";
		tempStream << _clientIt->contentLength;
		tmpEnv.push_back(tempStream.str());
		
		tmpVar = "REDIRECT_STATUS=CGI";
		tmpEnv.push_back(tmpVar);
		tmpVar = "POST_BODY=" + _clientIt->path; //to improve when you know how POST and body work
		tmpEnv.push_back(tmpVar);

	//putting vector into char **
		alilog << "the final env:\n";
		int	i = 0;
		for (std::vector<std::string>::iterator it = tmpEnv.begin(); it != tmpEnv.end(); ++it) {
			_env[i] = const_cast<char *>((*it).c_str());
			alilog << _env[i] << "\n";
			i++;
		}
		_env[i] = NULL;


//creating argv for execve
		const char	*argv[3];
		argv[0] = pathToExec.c_str();
		argv[1] = pathToScript.c_str();
		argv[2] = NULL;
		execve(argv[0], const_cast<char *const *>(argv), _env);
		selectStatusPage(500);
		alilog << "execve went wrong oops\n";
		alilog << errno;

	} else {
		alilog << "made it to parent yay\n";
		close(pipeFd[1]);
	//timeout management
		int status;
        int timePassed = 0;
        pid_t result;
        while (true) {
            result = waitpid(pid, &status, WNOHANG);
            if (result == 0 && timePassed <= 2000000) { //child is still alive and timeout limit it not reached
                usleep(100);
                timePassed += 100;
				continue;
            }
            if (result == 0 && timePassed > 2000000) { //child is still alive but timeout limit is reached 
                int killSuccessful = kill(pid, SIGTERM);
                if (killSuccessful == 0) {
                    selectStatusPage(500);
					close(pipeFd[0]);
					return;
            }
            if (result == -1) // child exited
                break;
        }
		alilog << "past the timeout\n";
		alilog << "what cgi spat out :\n";
		
//only if there wasnt timeout we should get here, so throw exceptions instead break higher
		char	buffer[1024];
		ssize_t	bytesRead;
		std::ofstream	cgiHtml("system/cgi.html");
		while ((bytesRead = read(pipeFd[0], buffer, 1023)) > 0) {
			buffer[bytesRead] = '\0';
			cgiHtml << buffer;
			alilog << buffer;
		}
		alilog << "\n";
		cgiHtml.close();
		close(pipeFd[0]);
		_clientIt->statusCode = 200;
		//_clientIt->requestFinished = true;
		_clientIt->sendPath = "system/cgi.html";
		// std::cout << "Child process exited with status: " << WEXITSTATUS(status) << std::endl;
	}
}
}

std::string Server::prependRoot(const std::string& path)
{
	if (path.find('/') == 0)
		return _root + path.substr(1);
	else
		return path;
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

std::string	Server::mimeType(const std::string& filepath)
{
	size_t		dotPosition;
	std::string extension;
	strMap_it	it;
	std::string defaultType = "application/octet-stream";

	dotPosition = filepath.find_last_of(".");
	if (dotPosition == std::string::npos)
		return defaultType;
	extension = filepath.substr(dotPosition);

	extension = fileExtension(filepath);
	it = _mimeTypes->find(extension);
	if (it != _mimeTypes->end())
		return it->second;
	return defaultType;
}
	
bool Server::dirListing(const std::string& path)
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

std::string Server::makeCookie(const std::string& key, const std::string& value, int expiration, const std::string& path)
{
	std::stringstream cookie;
	cookie << "set-cookie: " << key << "=" << value << ";";
	if (expiration >= 0)
		cookie << "max-age=" << expiration << ";";
	cookie << "path=" << path << ";";
	return cookie.str();
}

void Server::generateCookieLogPage()
{
	std::ofstream cookiePage(SITE_LOGPAGE, std::ios::binary | std::ios::trunc);
	if (cookiePage.fail())
	{
		cookiePage.close();
		throw std::runtime_error(E_TEMPFILE);
	}
	
	std::string cookieLogPath = "system/logs/" + _clientIt->sessionId + ".log";
	std::ifstream cookieLog(cookieLogPath.c_str());
	if (cookieLog.fail())
	{
		cookieLog.close();
		throw std::runtime_error(E_TEMPFILE);
	}

	cookiePage << "<!DOCTYPE html>";
	cookiePage << "<html>\n";
	cookiePage << "<head>\n";
	cookiePage << "<title>webserv - session log</title>\n";
	cookiePage << "<style>\n";
	cookiePage << "body {background-color: black; color: white; font-family: Arial, sans-serif; margin: 0; padding: 1% 0 0 0; text-align: left; display: flex;}\n";
	cookiePage << ".container {white-space: pre; flex: 1; position: relative; z-index: 2;}\n";
	cookiePage << "h1 {font-size: 42px;}\n";
	cookiePage << "p {font-size: 16px; line-height: 1.5; margin-right: 300px;}\n";
	cookiePage << "img {position: absolute; top: 0; right: 0; height: 100%; z-index: 1;}\n";
	//cookiePage << ".text-container{white-space: pre; position: absolute; top 0; left 0; right 0; z-index: 0; padding: 20px; background-color: black; color: white; font-family: Arial, sans-serif; overflow: auto;}\n";
	cookiePage << "</style>\n";
	cookiePage << "</head>\n";
	cookiePage << "<body>\n";
	cookiePage << "<div class=\"container\">\n";
	cookiePage << "<h1>" << "Log for session id " << _clientIt->sessionId << "</h1>\n";
	cookiePage << "<p>" << cookieLog.rdbuf() << "</p>\n";
	cookiePage << "</div>";
	cookiePage << "<img style=\"margin-left: auto;\" src=\"/catlockHolmes.png\">\n";
	cookiePage << "</body>\n";
	cookiePage << "</html>\n";
	cookiePage.close();
}