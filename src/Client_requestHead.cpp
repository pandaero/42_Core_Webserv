#include "../include/Client.hpp"

void Client::requestHead()
{
	if (_state > recv_reqHead) // done receiving request head CHANGE LATER
		return;

	if (_buffer.find("\r\n") == std::string::npos)
	{
		sendStatusPage(400);
		return;
	}
	parseRequestLine();
	if (_buffer.find("\r\n\r\n") == std::string::npos)
	{
		if (_buffer.size() >= MAX_HEADERSIZE) //This only makes sense if the readchunk is big enough.
			sendStatusPage(431);
		else
			sendStatusPage(400); // we read the entire "headers" but they weren't properly terminated.
		return;
	}
	parseRequestHeaders();
	handleSession();
	selectServerConfig();
	updateVars();
}

void Client::parseRequestLine()
{
	_method = splitEraseStr(_buffer, " ");
	_URL = splitEraseStr(_buffer, " ");
	_httpProtocol = splitEraseStr(_buffer, "\r\n");

	size_t questionMarkPos = _URL.find("?");
	if (questionMarkPos != std::string::npos)
	{
		_queryString = _URL.substr(questionMarkPos + 1);
		_URL = _URL.substr(0, questionMarkPos);
	}
}

void Client::parseRequestHeaders()
{
	_headers = parseStrMap(_buffer, ":", "\r\n", "\r\n");
	
	if (_headers.find("host") != _headers.end())
		_host = _headers["host"];
	
	if (_headers.find("content-length") != _headers.end())
		_contentLength = atoi(_headers["content-length"].c_str());
	
	if (_headers.find("content-type") != _headers.end())
		_contentType = _headers["content-type"];

	if (_headers.find("cookie") != _headers.end())
	{
		std::string temp = _headers["cookie"]; // parseStrMap erases from the input string. We want to preserve the cookie header to be able to pass it to CGI
		_cookies = parseStrMap(temp, "=", ";", "Please parse me to the end!");
	}
}

void Client::handleSession()
{
	if (_cookies.find(SESSIONID) != _cookies.end())
		_sessionId = _cookies[SESSIONID];
	else
	{
		_sessionId = generateSessionId();
		_setCookie = true;
	}

	std::string logPath = SYS_LOGS + _sessionId + ".log";
	std::ofstream logFile(logPath.c_str(), std::ios::app);
	if (logFile)
	{
		logFile << currentTime() << "\t" << inet_ntoa(_address.sin_addr) << "\t" << _method << " " << _URL << "\n";
		logFile.close();
	}
	else
	{
		logFile.close();
		std::cerr << E_C_OFSTREAM << std::endl;
	}
	generateSessionLogPage(logPath);
}

void Client::selectServerConfig()
{
	if (_host.empty() || stringInVec(_host, _config.getNames()))
	{
		_activeConfig = &_config;
		return;
	}
	for (size_t i = 0; i < _config.getAltConfigs().size(); ++i)
	{
		if (stringInVec(_host, _config.getAltConfigs()[i].getNames()))
		{
			_activeConfig = &_config.getAltConfigs()[i];
			return;
		}
	}
	_activeConfig = &_config;
	std::cout << "Hostname '" << _host << "' not found. Running default ServerConfig." << std::endl;
}

void Client::updateVars()
{
	_URL = ifDirAppendSlash(_URL);
	_directory = _URL.substr(0, _URL.find_last_of("/") + 1);
	_filename = _URL.substr(_URL.find_last_of("/") + 1);
	
	_dirListing = dirListing(_directory);
	
	strLocMap_it loc = _activeConfig->getLocations().find(_directory);
	if (loc == _activeConfig->getLocations().end())
		return; // this means request denied, but will not be dealt with here.
	
	_standardFile =  loc->second.std_file;
	if (_standardFile.empty())
		_standardFile = _activeConfig->getStandardFile();
	
	std::string	http_redir = loc->second.http_redir;
	if (!http_redir.empty())
		_updatedDirectory = http_redir;
	else if (_method == POST && !loc->second.upload_dir.empty())
		_updatedDirectory = loc->second.upload_dir;
	else
		_updatedDirectory = _directory;

	_updatedDirectory = prependRoot(_updatedDirectory); // need to preserve old dirs for dirlisting
	_updatedURL = _updatedDirectory + _filename;
}

void Client::requestHeadError()
{
	if (_httpProtocol != HTTPVERSION)
		sendStatusPage(505);
	else if (_method != GET && _method != POST && _method != DELETE)
		sendStatusPage(501);
	else if (_URL.find("/") == std::string::npos)
		sendStatusPage(404);
	else if (_contentLength > _activeConfig->getClientMaxBody())
		sendStatusPage(413); 
	else
	{
		strLocMap_it loc = _activeConfig->getLocations().find(_directory);
		
		if (loc == _activeConfig->getLocations().end()) // have to specifically allow each path in config file
			sendStatusPage(404); // returning 404, not 403, to not leak file structure
		else if ((_method == GET && !loc->second.get)
			|| (_method == POST && !loc->second.post)
			|| (_method == DELETE && !loc->second.delete_))
			sendStatusPage(405);
		else if (_method == POST && !resourceExists(_updatedDirectory)) // upload dir doesn't exist
			sendStatusPage(500);
	}
}
