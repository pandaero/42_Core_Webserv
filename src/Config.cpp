
#include "../include/Config.hpp"

/*
// Main Settings
	if (key == SERVERNAME)
		newServer.setName(value);
	else if (key == HOST)
		newServer.setHost(value);
	else if (key == PORT)
		newServer.setPort(value);
	
	// Bools
	else if (key == GET && value == "yes")
		newServer.setGet(true);
	else if (key == POST && value == "yes")
		newServer.setPost(true);
	else if (key == DELETE && value == "yes")
		newServer.setDelete(true);
	else if (key == DIRLISTING && value == "yes")
		newServer.setDirListing(true);
	
	// Directories
	else if (key == ROOT)
		newServer.setRoot(value);
	else if (key == DIR)
		newServer.setDir(value);
	else if (key == UPLOADDIR)
		newServer.setUploadDir(value);
	else if (key == CGIDIR)
		newServer.setCgiDir(value);
	else if (key == ERRORPAGE)
		newServer.setErrorPage(value);
	
	// Size restrictions
	else if (key == CLIMAXBODY)
		newServer.setClientMaxBody(value);
	else if (key == MAXCONNS)
		newServer.setMaxConnections(value);
	else if (key == BACKLOG)
		newServer.setBacklog(value);
*/

void setField(serverConfig& newConfig, std::string key, std::string value)
{
	// Main Settings
	if (key == SERVERNAME)
		newConfig.serverName = value;
	else if (key == HOST)
		newConfig.host = value;
	else if (key == PORT)
		newConfig.port = value;
	
	// Bools
	else if (key == GET && value == "yes")
		newConfig.get = true;
	else if (key == POST && value == "yes")
		newConfig.post = true;
	else if (key == DELETE && value == "yes")
		newConfig.delete_ = true;
	else if (key == DIRLISTING && value == "yes")
		newConfig.dirListing = true;
	
	// Directories
	else if (key == ROOT)
		newConfig.root = value;
	else if (key == DIR)
		newConfig.dir = value;
	else if (key == UPLOADDIR)
		newConfig.uploadDir = value;
	else if (key == CGIDIR)
		newConfig.cgiDir = value;
	else if (key == ERRORPAGE)
		newConfig.errorPage = value;
	
	// Size restrictions
	else if (key == CLIMAXBODY)
		newConfig.clientMaxBody = value;
	else if (key == MAXCONNS)
		newConfig.maxConnections = value;
	else if (key == BACKLOG)
		newConfig.backlog = value;
}

bool is_ServerBlock(std::string& line)
{
	size_t position = line.find("server");
	if (position != std::string::npos)
	{
		size_t nextChar = line.find_first_not_of(" \t\v\r\n", position);
		if (line[nextChar] != '{')
			return false;
		std::cout << position << std::endl;
		return true;
	}
	std::cout << line << std::endl;
	return false;
}

size_t findServerBlock(std::string input, size_t searchPosition)
{	
	while (searchPosition != std::string::npos)
	{
		size_t keyPosition = input.find("server", searchPosition);
		if (keyPosition == std::string::npos)
			break;
		size_t nextCharPosition = input.find_first_not_of(" \t\v\r\n", keyPosition + 6);
		if(input[nextCharPosition] == '{')
			return nextCharPosition + 1;
		searchPosition = nextCharPosition;
	}
	return std::string::npos;
}

std::vector<Server> setupServers(const char* path)
{
	std::ifstream		infile(path);
	std::stringstream	buffer;
	size_t				searchPosition = 0;
	std::string			key;
	std::string			value;
	std::vector<Server>	serverList;
	
	if (!infile.is_open())
		throw std::runtime_error(E_FILEOPEN);	
	buffer << infile.rdbuf();
	infile.close();
	size_t parsePosition = findServerBlock(buffer.str(), searchPosition);
	while (parsePosition != std::string::npos)
	{
		serverConfig newConfig;
		while (buffer.str()[parsePosition] != '}')
		{
			key = splitEraseStr(line, ":");
			value = trim(line);
			setField(newConfig, key, value);
		}
		Server newServer(newConfig);
		serverList.push_back(newServer);
		parsePosition = findServerBlock(buffer.str());
	}
	if (serverList.empty())
		throw std::runtime_error(E_NOSERVER);
	return serverList;
}

std::vector<Server> setupServers_old(const char* path)
{
	std::ifstream	infile(path);
	std::string		line;
	std::string		key;
	std::string		value;
	std::vector<Server>	serverList;
	
	if (!infile.is_open())
		throw std::range_error(E_FILEOPEN);		
	while (std::getline(infile, line))
	{
		if (trim(line) == "<server>")
		{
			serverConfig newConfig;
			while (std::getline(infile, line) && trim(line) != "</server>")
			{
				key = splitEraseStr(line, ":");
				value = trim(line);
				setField(newConfig, key, value);
			}
			Server newServer(newConfig);
			serverList.push_back(newServer);
		}
	}
	infile.close();
	return serverList;
}
