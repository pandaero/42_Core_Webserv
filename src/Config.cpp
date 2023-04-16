
#include "../include/Config.hpp"

void setField(Server& newServer, std::string key, std::string value)
{
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
}

std::vector<Server> setupServers(const char* path)
{
	std::ifstream	infile(path);
	std::string		line;
	std::string		key;
	std::string		value;
	std::vector<Server>	serverList;
	
	if (!infile.is_open())
		throw std::range_error("Could not open config file.");		
	while (std::getline(infile, line))
	{
		if (trim(line) == "<server>")
		{
			Server newServer;

			while (std::getline(infile, line) && trim(line) != "</server>")
			{
				key = splitEraseStr(line, ":");
				value = trim(line);
				setField(newServer, key, value);
			}
			serverList.push_back(newServer);
		}
	}
	infile.close();
	return serverList;
}
