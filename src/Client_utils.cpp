#include "../include/Client.hpp"

std::string Client::ifDirAppendSlash(const std::string& path)
{
	std::string newPath = path;
	
	if (isDirectory(prependRoot(newPath)) && newPath[newPath.size() - 1] != '/')
		newPath.append("/");
	return newPath;
}

std::string Client::prependRoot(const std::string& path)
{
	if (path.find('/') == 0)
		return _activeConfig->getRoot() + path.substr(1);
	else
		return path;
}

bool Client::dirListing(const std::string& directory)
{
	strLocMap_it loc = _activeConfig->getLocations().find(directory);
	
	if (loc == _activeConfig->getLocations().end())
		return false;
	if (loc->second.dir_listing == "yes")
		return true;
	if (loc->second.dir_listing == "no")
		return false;
	if (!_activeConfig->getDefaultDirlisting())
		return false;
	return true;
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

// try to use CGI on demand instead of this / store in string and be able to send that
// same for CGI output and statuspage
void Client::generateSessionLogPage(std::string logPath)
{
	/* std::ofstream sessionLogPage(SITE_LOGPAGE, std::ios::binary | std::ios::trunc);
	if (!sessionLogPage)
	{
		sessionLogPage.close();
		std::cerr << E_C_OFSTREAM << std::endl;
		return;
	} */
	
	std::ifstream logFile(logPath.c_str());
	if (!logFile)
	{
		logFile.close();
		std::cerr << E_C_IFSTREAM << std::endl;
		return;
	}

	_pageBuffer	<< "<!DOCTYPE html>\n<html>\n"
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
	//sessionLogPage.close();
	_sendFile = false;
}

void Client::generateStatusPage(int code)
{
/* 	std::ofstream errorPage(SYS_ERRPAGE, std::ios::binary | std::ios::trunc);
	if (!errorPage)
	{
		errorPage.close();
		std::cerr << E_C_OFSTREAM << std::endl;
		return;
	} */

	std::string httpMsg = getHttpMsg(code);

	_pageBuffer	<< "<!DOCTYPE html><html><head>\n"
				<< "<title>webserv - " << code << ": " << httpMsg << "</title>\n"
				<< "<style>\n"
				<< "body {background-color: black; color: white; font-family: Arial, sans-serif; margin: 0; padding: 5% 0 0 0; text-align: center;}\n"
				<< "h1 {font-size: 42px;}\n"
				<< "p {font-size: 16px; line-height: 1.5;}\n"
				<< "</style></head>\n"
				<< "<body>\n"
				<< "<h1>" << code << ": " << httpMsg << "</h1>\n"
				<< "<img style=\"margin-left: auto;\" src=\"https://http.cat/" << code << "\" alt=\"" << httpMsg << "\">\n"
				<< "</body>\n"
				<< "</html>\n";
	//errorPage.close();
	//_sendPath = SYS_ERRPAGE;
	_sendFile = false;
}

void Client::generateDirListingPage(const std::string& directory)
{
	/* std::ofstream dirListPage(SYS_DIRLISTPAGE);

	if (!dirListPage)
	{
		dirListPage.close();
		std::cerr << E_C_OFSTREAM << std::endl;
		return;
	} */

	_pageBuffer	<< "<head><title>Test Website for 42 Project: webserv</title><link rel=\"stylesheet\" type=\"text/css\" href=\"/styles.css\"/></head>"
				<< "<html><body><h1>Directory Listing</h1><ul>";

	struct dirent* ent;
	DIR* dir = opendir(directory.c_str());
	
	if (dir)
	{
		while ((ent = readdir(dir)) != NULL)
		{
			if (strcmp(ent->d_name, ".") == 0)
				continue;
			if (ent->d_type == DT_DIR) // append a slash if it's a directory
				_pageBuffer << "<li><a href=\"" << _directory + ent->d_name << "/\">" << ent->d_name << "/</a></li>";
			else
				_pageBuffer << "<li><a href=\"" << _directory + ent->d_name << "\">" << ent->d_name << "</a></li>";
		}
		closedir(dir);
	}

	_pageBuffer << "</ul></body></html>";
	//dirListPage.close();
	_sendFile = false;
}

void Client::isCgiRequest()
{
	std::string extension = fileExtension(_filename);
	if (_activeConfig->getCgiPaths().find(extension) == _activeConfig->getCgiPaths().end())
		_cgiRequest = false;
	else
	{
		_cgiExecPath = _activeConfig->getCgiPaths()[extension];
		_cgiRequest = true;
	}
}

std::string Client::mimeType(const std::string& filepath)
{
	std::string extension;
	strMap_it	it;
	std::string defaultType = "application/octet-stream";

	extension = fileExtension(filepath);
	it = _activeConfig->getMIMETypes()->find(extension);
	if (it != _activeConfig->getMIMETypes()->end())
		return it->second;
	return defaultType;
}
