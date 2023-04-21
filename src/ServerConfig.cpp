#include "../include/ServerConfig.hpp"

ServerConfig::ServerConfig()
{
	setDefault();
}
ServerConfig::ServerConfig(std::string input)
{
	setDefault();
	parseInput(input);
}

ServerConfig::~ServerConfig()
{}

void ServerConfig::setDefault()
{
	serverName = "unnamedServer";
	host = "0";
	port = "0";
	get = false;
	post = false;
	delete_ = false;
	dirListing = false;
	root = "website/root";
	dir = "website/dir";
	uploadDir = "website/upload";
	cgiDir = "website/cgi";
	errorPage = "website/error.html";
	clientMaxBody = "10000";
	maxConnections = "1000";
	backlog = "100";
}

void ServerConfig::parseInput(std::string input)
{
	std::string		key;
	std::string		value;
	
	while (!trim(input).empty())
	{
		key = splitEraseStr(input, " ");
		trim(input);
		value = splitEraseStr(input, ";");
		setField(key, value);
	}
}

void ServerConfig::setField(std::string key, std::string value)
{
	// Main Settings
	if (key == SERVERNAME)
		serverName = value;
	else if (key == HOST)
		host = value;
	else if (key == PORT)
		port = value;
	
	// Bools
	else if (key == GET && value == "yes")
		get = true;
	else if (key == POST && value == "yes")
		post = true;
	else if (key == DELETE && value == "yes")
		delete_ = true;
	else if (key == DIRLISTING && value == "yes")
		dirListing = true;
	
	// Directories
	else if (key == ROOT)
		root = value;
	else if (key == DIR)
		dir = value;
	else if (key == UPLOADDIR)
		uploadDir = value;
	else if (key == CGIDIR)
		cgiDir = value;
	else if (key == ERRORPAGE)
		errorPage = value;
	
	// Size restrictions
	else if (key == CLIMAXBODY)
		clientMaxBody = value;
	else if (key == MAXCONNS)
		maxConnections = value;
	else if (key == BACKLOG)
		backlog = value;
}

// UTIL FUNCTIONS

std::vector<ServerConfig> getConfigs(const char* path)
{
	std::string					configData;
	std::vector<ServerConfig>	configList;
	
	configData = configFileToString(path);
	trim(configData);
	while (!configData.empty())
	{
		ServerConfig newConfig(getConfigElement(configData));
		configList.push_back(newConfig);
	}
	std::cout << "Finished parsing input. " << configList.size() << " config elements built." << std::endl;
	if (configList.empty())
		throw std::runtime_error(E_NOSERVER);
	return configList;
}


std::string configFileToString(const char* path)
{
	std::ifstream		infile(path);
	std::stringstream	buffer;
	
	if (!infile.is_open())
	{
		std::string invalidpath(path);
		throw std::runtime_error(E_FILEOPEN + invalidpath);
	}
	buffer << infile.rdbuf();
	infile.close();
	return buffer.str();
}

/*
For now, only accepts "server" elements. Probably sufficient for the subject.
But quickly expandable if we have to add other elements.

Does not allow declaration of subelements at this point.
*/
std::string getConfigElement(std::string& configString)
{
	std::string		elementTitle;
	std::string		elementBody;
	size_t			len_close;
	
	elementTitle = splitEraseStr(configString, "{");
	if (trim(elementTitle) != "server")
		throw std::runtime_error(E_ELMNTDECL + elementTitle);
	len_close = configString.find("}");
	if (configString.find("{") < len_close)
		throw std::runtime_error(E_SUBELEMNT + configString);
	elementBody = splitEraseStr(configString, "}");
	return trim(elementBody);
}
