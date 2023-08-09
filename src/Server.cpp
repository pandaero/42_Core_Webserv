#include "../include/Server.hpp"

Server::Server(const ServerConfig & config):
	_pollStructs(NULL)
{
	// Set main values stored in configPairs
	strMap		configPairs = config.getConfigPairs();
	setNames(configPairs.find(SERVERNAME)->second);
	setHost(configPairs.find(HOST)->second);
	setPort(configPairs.find(PORT)->second);
	setRoot(configPairs.find(ROOT)->second);
	setClientMaxBody(configPairs.find(CLIMAXBODY)->second);
	setMaxConnections(configPairs.find(MAXCONNS)->second);
	setDefaultDirListing(configPairs.find(DIRLISTING)->second);
	setMIMEtype(); // should do this for webserv, not for each server. maybe implement that later
	//setBacklog(config.backlog); need this?!
	
	// Copy remaining values directly to server variables 
	_errorPagesPaths = config.getErrorPaths();
	_locations = config.getLocations();
	_cgiPaths = config.getCgiPaths();

	// Init polling structs
	_pollStructs = new pollfd[_maxConns];
	for (size_t i = 0; i < _maxConns; i++)
	{
		_pollStructs[i].fd = -1;
		_pollStructs[i].events = 0;
		_pollStructs[i].revents = 0;
	}
}

void	Server::startListening()
{
	ANNOUNCEME
	int	options = 1;
	
	_server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_server_fd == -1)
		throw	socketCreationFailureException();
	
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

	_pollStructs[0].fd = _server_fd;
	_pollStructs[0].events = POLLIN;
	_pollStructs[0].revents = 0;

	// init tables?
}

void Server::cleanup()
{
	for (clientVec_it it = _clients.begin(); it != _clients.end(); ++it)
		closeClient(it);
	if (_server_fd != -1)
		close(_server_fd);
	if (_pollStructs)
		delete [] _pollStructs;
}

void Server::poll()
{
	ANNOUNCEME
	if (::poll(_pollStructs, _clients.size() + 1, -1) == -1)
		throw (E_POLL);
}

void Server::acceptConnections()
{
	ANNOUNCEME
	if (_pollStructs[0].revents & POLLIN)
	{
		int		index = freePollStructIndex();
		
		if (index == -1)
		{
			std::cerr << I_CONNECTIONLIMIT << std::endl;
			return;
		}
		_clients.push_back(Client(index));
		
		Client&	newClient = _clients.back();
		int 	addrlen = sizeof(_serverAddress);

		// while loop to deal with all incoming connections until -1 triggers return
		while (true)
		{
			int new_sock = accept(_server_fd, (sockaddr*)newClient.sockaddr(), (socklen_t*)&addrlen);
			if (new_sock == -1)
			{
				if (errno != EWOULDBLOCK)
					std::cerr << E_ACCEPT << std::endl;
				return;
			}
			newClient.setSocketfd(new_sock);
			_pollStructs[index].fd = new_sock;
			_pollStructs[index].events = POLLIN | POLLHUP;
			std::cout << "New client accepted on fd " << new_sock << "." << std::endl;
			std::cout << "Clients size: " << _clients.size() << std::endl;
		}
	}
}

void Server::receiveData()
{
	ANNOUNCEME
	char	buffer[RECV_CHUNK_SIZE];
	
	bzero(buffer, RECV_CHUNK_SIZE);
	_bytesReceived = recv(_currentClientfd, buffer, RECV_CHUNK_SIZE, 0);
	std::cout << _bytesReceived << " bytes received.\nContent:\n" << buffer << std::endl;
	
	if (_bytesReceived <= 0)
	{
		closeClient(_currentClientIt);
		throw (I_CLOSENODATA); //prolly gonna throw a real exception here
	}
	_currentClientIt->buffer().append(buffer);
}

void Server::handleConnections()
{
	ANNOUNCEME
	for (clientVec_it clientIt = _clients.begin(); clientIt != _clients.end(); ++clientIt)
	{
		ANNOUNCEME
		if (_clients.empty())
		{
			std::cout << "How in the fuck?" << std::endl;
			return;
		}
		_currentClientfd = clientIt->socketfd();
		_currentClientIt = clientIt;
		if (_pollStructs[clientIt->pollStructIndex()].revents & (POLLIN | POLLHUP))
		{
			try
			{
				receiveData();
				handleRequestHead();
				handleRequestBody();
				sendResponse();
				//send response after cmopleting head and body
				
				
				// bs, just to send stuff
				sendStatusCodePage(200);
			}
			catch(const char* msg)
			{
				std::cerr << msg << _statuscode << ": " << getHttpMsg(_statuscode) << std::endl;
			}
		}
	}
}

bool Server::requestError()
{
	_statuscode = 0;
	// wrong protocol
	if (_currentClientIt->httpProtocol() != HTTPVERSION)
		return (_statuscode = 505);
	// method not supported by server
	if (_currentClientIt->method() != GET
		&& _currentClientIt->method() != POST
		&& _currentClientIt->method() != DELETE)
		return (_statuscode = 501);
	// body size too large
	if (_currentClientIt->contentLength() > (int)_clientMaxBody)
		return (_statuscode = 413);
	// access forbidden (have to specifically allow access in config file)
	std::cout << "client->directory:'" << _currentClientIt->directory() << "'" << std::endl;
	strLocMap_it locIt = _locations.find(_currentClientIt->directory());
	if (locIt == _locations.end())
		return (_statuscode = 404); // only returning 404 (and not 403) to not leak file structure
	// acces granted, but not for the requested method
	if (_currentClientIt->method() == GET && !locIt->second.get
		|| _currentClientIt->method() == POST && !locIt->second.post
		|| _currentClientIt->method() == DELETE && !locIt->second.delete_)
		return (_statuscode = 405);
	
	
	
	if (_statuscode)
	{
		// non bool strcutrue
		// senderrormsg
		// throw error
	}
	return false;
}

void Server::handleRequestHead()
{
	ANNOUNCEME
	if (_currentClientIt->requestHeadComplete())
		return;
	if (_currentClientIt->buffer().size() > MAX_HEADERSIZE)
	{
		sendStatusCodePage(431); //obviously not cool, integrate this. can be done with comment below, prolly throw a real exception
		closeClient(_currentClientIt);
		throw (I_REQUESTHEADERROR);
	}
	if (_currentClientIt->buffer().find("\r\n\r\n") != std::string::npos)
	{
		_currentClientIt->buildRequest();
		std::cout << "request path raw:'" << _currentClientIt->path() << "'" << std::endl;
		if (requestError())
		{
			sendStatusCodePage(_statuscode);
			closeClient(_currentClientIt);
			throw (I_REQUESTHEADERROR);
			
			// maybe most elegant: set errorcode and just continue process (or throw real excepetion). when send starts and finds error it fucks off
			// process rest of requst it would see error code and then instead just send that
		}
		// handle the request

	}
	throw ("request head not complete, skipping rest of handleConnections loop");
	// if not complete, have to skip rest of shmisms. maybe better to put guard clause in the others
	// or can throw...
}

void Server::handleRequestBody()
{
	if (_currentClientIt->requestBodyComplete())
		return;
	// process body

/* 	WRITETOFILE
	// throw not yet caught
	void Client::writeBodyToFile()
	{
		//now just taking request path, have to modify this with the config file given directory
		std::string		writePath(_request.path());

		std::ofstream	outputFile(writePath.c_str(), std::ios::binary | std::ios::app);
		
		if (!outputFile.is_open())
			throw (E_REQUESTFILE);
		outputFile.write(_buffer.c_str(), _buffer.size());
		outputFile.close();
		_request.addToBodyBytesWritten(_buffer.size()); // outputFile.tellp() delta prolly better but nah.
		chunkhandler(TM).
	} */
}

std::string Server::buildHeader(int code, std::string filePath)
{
	std::stringstream	ss_header;
	
	ss_header << HTTPVERSION << ' ' << code << ' ' << getHttpMsg(code) << "\r\n";
	ss_header << "Server: " << SERVERVERSION << "\r\n";
	// select real contenttype!
	ss_header << "content-type: text/html; charset=utf-8" << "\r\n";
	ss_header << "content-length: " << getFileSize(filePath) << "\r\n";
	ss_header << "\r\n";
	return ss_header.str();
}

void Server::sendFile(std::string path)
{

}

void Server::sendResponse()
{
	ANNOUNCEME
	int					fileBytesSent = 0;
	int					closingBytesSent = 0;
	std::stringstream	ss_header;
	
	// simple get request


	// error checking already handled
	// location is known
	// method is allowed

	// select file to send
	std::string	completePath(_root + _currentClientIt->path());
	std::string	pathToSend;
	std::cout << "completePath(root + clientPath):'" << completePath << "'" << std::endl;

	if (isDirectory(completePath))
	{
		std::cout << "completePath is a directory" << std::endl;
		std::string	_standardServeFileName = "index.html"; //change config to specifiable filename later!

		if (resourceExists(completePath + _standardServeFileName))
		{
			std::cout << "yes, '" << completePath + _standardServeFileName << "' exists." << std::endl;
			_statuscode = 200;
			pathToSend = completePath + _standardServeFileName;
		}
		else if (dirListing(completePath))
		{
			// send dirlisting and get outta here (at least for now)
			_statuscode = 200;
			std::cout << "no, '" << completePath + _standardServeFileName << "' does not exist, but dir listing is allowed. Show dir listing here" << std::endl;
		}
		else
		{
			std::cout << "no, '" << completePath + _standardServeFileName << "' does not exist, and dir listing is not allowed. Send 404." << std::endl;
			//send 404, can only set here (atm at least)
			_statuscode = 404;
		}
	}
	else // is not directory
	{
		if (resourceExists(completePath))
		{
			_statuscode = 200;
			pathToSend = completePath;
		}

			//send file
		else
		{
			_statuscode = 404;
			// get out / throw w/e
		}
	}
	
	if (_currentClientIt->method() == GET || _currentClientIt->method() == DELETE)
	{
		std::ifstream	file;
		//should be completepath?
		file.open(_currentClientIt->path().c_str(), std::ios::binary);
		// file not accessible - we treat it as file not found. Maybe more specific behavior? Wr already checked folder permissions tho!
		if (file.fail())
		{
			file.close();
			return (_statuscode = 404);
		}
		
	}



	

	if (::send(_currentClientfd, ss_header.str().c_str(), ss_header.str().size(), 0) == -1)
		throw (E_SEND);

	

	std::ifstream	file;
	// Send file or corresponding status page.
	if (response._statusCode == 200)
		file.open(response.getFilePath().c_str(), std::ios::binary);
	else
		file.open(getStatusPage(response._statusCode).c_str(), std::ios::binary);
	if (file.fail())
		std::cerr << "Error: Response: send: could not open file." << std::endl;
	char	buffer[1];
	// DEBUG
	std::cout << "sending file: " << response.getFilePath() << std::endl;
	while (file.read(buffer, sizeof(buffer)))
	{
		if ((fileBytesSent += ::send(socketfd, buffer, file.gcount(), 0)) == -1)
		{
			std::cerr << "Error: Response: send: could not send file data.";
			
		}
		// DEBUG
		std::cout << "the buffer:\n" << buffer << std::endl;
	}
	// if (!file.eof())
	// {
	// 	std::cerr << "Error: Response: send: could not read entire file.";
	// 	return (-1);
	// }
	// DEBUG
	std::cout << "Reached EOF" << std::endl;
	file.close();
	// Send termination CRLFs
	std::string	terminationSequence(TERMINATION);
	if ((closingBytesSent += ::send(socketfd, terminationSequence.data(), terminationSequence.size(), 0)) == -1)
	{
		std::cerr << "Error: Response: send: failure to send termination data.";
	}
	// DEBUG
	std::cout << "Response sent, " << fileBytesSent << " bytes from file." << std::endl;
	//return (fileBytesSent);
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

int Server::freePollStructIndex()
{
	size_t i = 0;
	
	while (i < _maxConns && _pollStructs[i].fd != -1)
		i++;
	if (i == _maxConns)
		i = -1;
	return i;
}

void Server::sendStatusCodePage(int code)
{
	Response	response(code, _names);
		
	//gotta check for client supplied error page and try to send that. only if not supplied or fail, send this
	//use getStatusPage in server for this, also rename shit, too ambiguous
	if (::send(_currentClientfd, response.getStatusPage(), response.getSize(), 0) == -1)
		throw (E_SEND);
}

void Server::closeClient(clientVec_it clientIt)
{
	ANNOUNCEME
	int	pollStructIndex = clientIt->pollStructIndex();
	
	close(_currentClientfd);
	_pollStructs[pollStructIndex].fd = -1;
	_pollStructs[pollStructIndex].events = 0;
	_pollStructs[pollStructIndex].revents = 0;
	std::cout << "closeClient on fd " << _currentClientfd << std::endl;
	_clients.erase(clientIt);
}

void Server::sendResponse_old(Response response, int socketfd)
{
	int	fileBytesSent = 0;
	int	closingBytesSent = 0;

	ANNOUNCEME
	std::cout << "Header:\n" << response._responseHeader << std::endl;
	// Send header
	if (::send(socketfd, response._responseHeader.data(), response._responseHeader.size(), 0) == -1)
		std::cerr << "Error: Server::sendResponse: send: failure to send header data.";
	std::ifstream	file;
	// Send file or corresponding status page.
	if (response._statusCode == 200)
		file.open(response.getFilePath().c_str(), std::ios::binary);
	else
		file.open(getStatusPage(response._statusCode).c_str(), std::ios::binary);
	if (file.fail())
		std::cerr << "Error: Response: send: could not open file." << std::endl;
	char	buffer[1];
	// DEBUG
	std::cout << "sending file: " << response.getFilePath() << std::endl;
	while (file.read(buffer, sizeof(buffer)))
	{
		if ((fileBytesSent += ::send(socketfd, buffer, file.gcount(), 0)) == -1)
		{
			std::cerr << "Error: Response: send: could not send file data.";
			
		}
		// DEBUG
		std::cout << "the buffer:\n" << buffer << std::endl;
	}
	// if (!file.eof())
	// {
	// 	std::cerr << "Error: Response: send: could not read entire file.";
	// 	return (-1);
	// }
	// DEBUG
	std::cout << "Reached EOF" << std::endl;
	file.close();
	// Send termination CRLFs
	std::string	terminationSequence(TERMINATION);
	if ((closingBytesSent += ::send(socketfd, terminationSequence.data(), terminationSequence.size(), 0)) == -1)
	{
		std::cerr << "Error: Response: send: failure to send termination data.";
	}
	// DEBUG
	std::cout << "Response sent, " << fileBytesSent << " bytes from file." << std::endl;
	//return (fileBytesSent);
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

void Server:: setRoot(std::string input)
{
	// checkMethodAccess(input);
	_root = input;
}

void Server::setDir(std::string input)
{
	// checkMethodAccess(input);
	_dir = input;
}

void Server::setUploadDir(std::string input)
{
	// checkWriteAccess(input);
	_uploadDir = input;
}

void Server::setCgiDir(std::string input)
{
	// checkExecAccess(input);
	_cgiDir = input;
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

void Server::setBacklog(std::string input)
{
	if (input.find_first_not_of("0123456789") != std::string::npos)
			throw std::runtime_error(E_BACKLOGINPUT + input + '\n');
	_backlog = atoi(input.c_str());
	if (_backlog > MAX_BACKLOG)
		throw std::runtime_error(E_BACKLOGVAL + input + '\n');
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
	std::string defaultType = "application/octet-stream";

	dotPosition = filepath.find_last_of(".");
	if (dotPosition == std::string::npos)
		return defaultType;
	extension = filepath.substr(dotPosition + 1);
	if (_mimeTypes.find(extension) != _mimeTypes.end())
		return _mimeTypes[extension];
	return defaultType;
}


void Server::setMIMEtype()
{
	_mimeTypes[".html"] = "text/html";
	_mimeTypes[".htm"] = "text/html";
	_mimeTypes[".css"] = "text/css";
	_mimeTypes[".js"] = "application/javascript";
	_mimeTypes[".json"] = "application/json";
	_mimeTypes[".jpg"] = "image/jpeg";
	_mimeTypes[".jpeg"] = "image/jpeg";
	_mimeTypes[".png"] = "image/png";
	_mimeTypes[".gif"] = "image/gif";
	_mimeTypes[".bmp"] = "image/bmp";
	_mimeTypes[".ico"] = "image/x-icon";
	_mimeTypes[".svg"] = "image/svg+xml";
	_mimeTypes[".xml"] = "application/xml";
	_mimeTypes[".pdf"] = "application/pdf";
	_mimeTypes[".zip"] = "application/zip";
	_mimeTypes[".gz"] = "application/gzip";
	_mimeTypes[".tar"] = "application/x-tar";
	_mimeTypes[".mp4"] = "video/mp4";
	_mimeTypes[".mpeg"] = "video/mpeg";
	_mimeTypes[".avi"] = "video/x-msvideo";
	_mimeTypes[".ogg"] = "audio/ogg";
	_mimeTypes[".mp3"] = "audio/mpeg";
	_mimeTypes[".wav"] = "audio/wav";
	_mimeTypes[".mov"] = "video/quicktime";
	_mimeTypes[".ppt"] = "application/vnd.ms-powerpoint";
	_mimeTypes[".xls"] = "application/vnd.ms-excel";
	_mimeTypes[".doc"] = "application/msword";
	_mimeTypes[".csv"] = "text/csv";
	_mimeTypes[".txt"] = "text/plain";
	_mimeTypes[".rtf"] = "application/rtf";
	_mimeTypes[".shtml"] = "text/html";
	_mimeTypes[".php"] = "application/php";
	_mimeTypes[".jsp"] = "text/plain";
	_mimeTypes[".swf"] = "application/x-shockwave-flash";
	_mimeTypes[".ttf"] = "application/x-font-truetype";
	_mimeTypes[".eot"] = "application/vnd.ms-fontobject";
	_mimeTypes[".woff"] = "application/font-woff";
	_mimeTypes[".woff2"] = "font/woff2";
	_mimeTypes[".ics"] = "text/calendar";
	_mimeTypes[".vcf"] = "text/x-vcard";
	_mimeTypes[".mid"] = "audio/midi";
	_mimeTypes[".midi"] = "audio/midi";
	_mimeTypes[".wmv"] = "video/x-ms-wmv";
	_mimeTypes[".webm"] = "video/webm";
	_mimeTypes[".3gp"] = "video/3gpp";
	_mimeTypes[".3g2"] = "video/3gpp2";
	_mimeTypes[".pl"] = "text/plain";
	_mimeTypes[".py"] = "text/x-python";
	_mimeTypes[".java"] = "text/x-java-source";
	_mimeTypes[".c"] = "text/x-c";
	_mimeTypes[".cpp"] = "text/x-c++";
	_mimeTypes[".cs"] = "text/plain";
	_mimeTypes[".rb"] = "text/x-ruby";
	_mimeTypes[".htm"] = "text/html";
	_mimeTypes[".shtml"] = "text/html";
	_mimeTypes[".xhtml"] = "application/xhtml+xml";
	_mimeTypes[".m4a"] = "audio/mp4";
	_mimeTypes[".mp4a"] = "audio/mp4";
	_mimeTypes[".oga"] = "audio/ogg";
	_mimeTypes[".ogv"] = "video/ogg";
	_mimeTypes[".ogx"] = "application/ogg";
	_mimeTypes[".oga"] = "audio/ogg";
	_mimeTypes[".m3u8"] = "application/vnd.apple.mpegurl";
	_mimeTypes[".qt"] = "video/quicktime";
	_mimeTypes[".ts"] = "video/mp2t";
	_mimeTypes[".xl"] = "application/excel";
	_mimeTypes[".cab"] = "application/vnd.ms-cab-compressed";
	_mimeTypes[".msi"] = "application/x-msdownload";
	_mimeTypes[".dmg"] = "application/x-apple-diskimage";
	_mimeTypes[".exe"] = "application/octet-stream";
	_mimeTypes[".bin"] = "application/octet-stream";
	_mimeTypes[".ps"] = "application/postscript";
	_mimeTypes[".so"] = "application/octet-stream";
	_mimeTypes[".dll"] = "application/octet-stream";
	_mimeTypes[".m4v"] = "video/x-m4v";
	_mimeTypes[".ser"] = "application/java-serialized-object";
	_mimeTypes[".sh"] = "application/x-sh";
	_mimeTypes[".log"] = "text/plain";
	_mimeTypes[".diff"] = "text/x-diff";
	_mimeTypes[".patch"] = "text/x-diff";
	_mimeTypes[".xhtml"] = "application/xhtml+xml";
	_mimeTypes[".php"] = "application/x-httpd-php";
	_mimeTypes[".plist"] = "application/xml";
	_mimeTypes[".sln"] = "text/plain";
	_mimeTypes[".tiff"] = "image/tiff";
	_mimeTypes[".app"] = "application/octet-stream";
	_mimeTypes[".ics"] = "text/calendar";
}

std::string Server::getStatusPage(int code) const
{
	if (_errorPagesPaths.find(code) != _errorPagesPaths.end())
		return _errorPagesPaths.find(code)->second;
	else
		return _errorPagesPaths.find(-1)->second;
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
