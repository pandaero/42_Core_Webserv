#include "../include/Client.hpp"

Client::Client(const ServerConfig& config):
	_config(config), _activeConfig(&config), _fd(-42), _statusCode(0), _filePosition(0), _bytesWritten(0),
	_contentLength(0), _state(recv_head), _dirListing(false), _append(false),
	_setCookie(false), _childLaunched(false), _cgiRequest(false)
{
	
}

void Client::whoIsI()
{
	std::stringstream title;
	title << "----- Client on fd " << _fd << " with session id " << _sessionId << " -----";
	std::string separator(title.str().size(), '-');
	
	std::cout	<< "\n" << title.str() << "\n"
				<< "host: " << _host << "\n"
				<< "method: " << _method << "\n"
				<< "URL: " << _URL << "\n"
				<< "dir: " << _directory << "\n"
				<< "file: " << _filename << "\n"
				<< "content-length: " << _contentLength << "\n"
				<< "content-type: " << _contentType << "\n"
				<< "standardfile: " << _standardFile << "\n"
				<< "dirlisting: " << (_dirListing ? "yes" : "no") << "\n"
				<< "updatedURL: " << _updatedURL << "\n"
				<< "updatedDirectory: " << _updatedDirectory << "\n"
				<< "query string: " << _queryString << "\n"
				<< separator << "\n" << std::endl;
}

void Client::incomingData()
{

}

bool Client::requestHead()
{
	if (_state > recv_head) // done receiving request head CHANGE LATER
		return true;

	if (_buffer.find("\r\n") == std::string::npos)
		return (sendStatusPage(400), false);
	
	parseRequestLine();
	
	if (_buffer.find("\r\n\r\n") == std::string::npos)
	{
		if (_buffer.size() >= MAX_HEADERSIZE) //This only makes sense if the readchunk is big enough.
			return (sendStatusPage(431), false);
		else
			return (sendStatusPage(400), false); // we read the entire "headers" but they weren't properly terminated.
	}

	parseRequestHeaders();
	selectServerConfig();
	updateClientVars();
	handleSession();
	if (requestError())
		return false;
	
	// check for body
	if (_clientIt->method != POST) // we don't process bodies of GET or DELETE requests
		_clientIt->state = handleRequest;
	else if (_clientIt->contentLength <= _clientIt->buffer.size()) // body is already complete in this recv (header content has already been deleted from buffer)
		_clientIt->state = handleRequest;
	else
		_clientIt->state = recv_body;

	_clientIt->cgiRequest = cgiRequest();
	_clientIt->whoIsI();
	return true;
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

void Client::updateClientVars()
{
	_URL = ifDirAppendSlash(_URL);
	_directory = _URL.substr(0, _URL.find_last_of("/") + 1);
	_filename = _URL.substr(_URL.find_last_of("/") + 1);
	
	_dirListing = dirListing(_directory);
	
	// select the file to try to serve in case of directory
	_standardFile = _locations[_directory].std_file;
	if (_standardFile.empty())
		_standardFile = _standardFile;
	
	// check for HTTP redirection and upload redirection
	std::string	http_redir = _locations[_directory].http_redir;
	if (!http_redir.empty())
		_updatedDirectory = http_redir;
	else if (_method == POST && !_locations[_directory].upload_dir.empty())
		_updatedDirectory = _locations[_directory].upload_dir;
	else
		_updatedDirectory = _directory;

	_updatedDirectory = prependRoot(_updatedDirectory);
	_updatedURL = _updatedDirectory + _filename;
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

// try to use CGI on demand instead of this
void Client::generateSessionLogPage(std::string logPath)
{
	std::ofstream sessionLogPage(SITE_LOGPAGE, std::ios::binary | std::ios::trunc);
	if (!sessionLogPage)
	{
		sessionLogPage.close();
		std::cerr << E_C_OFSTREAM << std::endl;
		return;
	}
	
	std::ifstream logFile(logPath.c_str());
	if (!logFile)
	{
		logFile.close();
		std::cerr << E_C_IFSTREAM << std::endl;
		return;
	}

	sessionLogPage	<< "<!DOCTYPE html>\n<html>\n"
					<< "<head>\n"
					<< "<title>webserv - session log</title>\n"
					<< "<link rel=\"stylesheet\" type=\"text/css\" href=\"styles.css\"/>\n"
					<< "</head>\n"
					<< "<body>\n"
					<< "<div class=\"logContainer\">\n"
					<< "<h2>" << "Log for session id " << _sessionId << "</h2>\n"
					<< "<logtext>" << logFile.rdbuf() << "</logtext>\n"
					<< "</div>\n"
					<< "<img style=\"margin-left: auto; position: fixed; top: 0; right: 0; height: 70%; z-index: 1;\" src=\"/img/catlockHolmes.png\">\n"
					<< "</body>\n</html>\n";
	sessionLogPage.close();
}

bool Client::requestError()
{
	if (_httpProtocol != HTTPVERSION)
		return (sendStatusPage(505), true);
	
	if (_method != GET
		&& _method != POST
		&& _method != DELETE)
		return (sendStatusPage(501), true);

	if (_URL.find("/") == std::string::npos)
		return (sendStatusPage(404), true);

	if (_contentLength > _clientMaxBody)
		return (sendStatusPage(413), true); 
	
	// check requested resource
	strLocMap_it locIt = _locations.find(_directory);
		
	// access forbidden (have to specifically allow each path in config file)
	if (locIt == _locations.end())
		return (sendStatusPage(404), true); // returning 404, not 403, to not leak file structure

	if ((_method == GET && !locIt->second.get)
		|| (_method == POST && !locIt->second.post)
		|| (_method == DELETE && !locIt->second.delete_))
		return (sendStatusPage(405), true);
	
	return false;
}
