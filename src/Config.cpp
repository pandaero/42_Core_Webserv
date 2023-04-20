
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

std::vector<Server> setupServers(const char* path)
{
	std::ifstream		infile(path);
	std::stringstream	buffer;
	std::string			key;
	std::string			value;
	std::vector<Server>	serverList;
	
	if (!infile.is_open())
		throw std::runtime_error(E_FILEOPEN);
	buffer << infile.rdbuf();
	infile.close();
	std::string configFile = buffer.str();
	std::string blockTitle = splitEraseStr(configFile, "{");
	if (trim(blockTitle) != "server")
		throw std::runtime_error(E_BLOCTITLE);
	int i = 0;
	while (!trim(configFile).empty())
	{
		
		std::cout << i++ << std::endl;
		serverConfig newConfig;
		while (trim(configFile)[0] != '}')
		{
			key = splitEraseStr(configFile, " ");
			trim(configFile);
			value = splitEraseStr(configFile, ";");
			std::cout << "key: " << key << std::endl;
			std::cout << "value: " << value << std::endl;
			setField(newConfig, key, value);
		}
		Server newServer(newConfig);
		serverList.push_back(newServer);
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
