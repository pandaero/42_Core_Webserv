#include "../include/ServerConfig.hpp"

ServerConfig::ServerConfig(std::string serverConfigStr)
{
	// Parse default config
	std::string	defaultConfigData, defaultConfigElement;
	std::string	key, value;
	
	defaultConfigData = configFileToString(DEFAULTCONFIGPATH);
	defaultConfigElement = getConfigElement(defaultConfigData);	
	while (!trim(defaultConfigElement).empty())
	{
		key = splitEraseChars(defaultConfigElement, " \t");
		trim(defaultConfigElement);
		value = splitEraseStr(defaultConfigElement, ";");
		_configPairs.insert(std::make_pair(key, value));
	}
	// Overwrite default with config
	while (!trim(serverConfigStr).empty())
	{
		key = splitEraseChars(serverConfigStr, " \t");
		trim(serverConfigStr);
		value = splitEraseStr(serverConfigStr, ";");
		if (_configPairs.find(key) != _configPairs.end())
		{
			_configPairs.erase(_configPairs.find(key));
			_configPairs.insert(make_pair(key, value));
		}
	}
}

ServerConfig::~ServerConfig(){}

/*
void ServerConfig::setField(std::string key, std::string value)
{
	// Main Settings
	if (key == SERVERNAME)
		serverName = value;
	else if (key == HOST)
		host = value;
	else if (key == PORT)
		port = value;
	
	// Bools (these ifs are split to evade the last else)
	else if (key == GET)
	{
		if (value == "yes")
			get = true;
		else
			get = false;
	}
	else if (key == POST)
	{
		if (value == "yes")
			post = true;
		else
			post = false;
	}
	else if (key == DELETE)
	{
		if (value == "yes")
			delete_ = true;
		else
			delete_ = false;
	}
	else if (key == DIRLISTING)
	{
		if (value == "yes")
			dirListing = true;
		else
			dirListing = false;
	}
	
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
	{
		std::string element;
		std::vector<size_t> errorNumbers;
		
		element = splitEraseChars(value, " ");
		trim(element);
		while (element.find_first_not_of("0123456789") == std::string::npos)
		{
			size_t errNum = atoi(element.c_str());
			if (errNum < 100 || errNum > 599)
				throw std::runtime_error(E_INVALERRNUM + element + '\n');
			errorNumbers.push_back(errNum);
			element = splitEraseChars(value, " ");
			trim(element);
		}
		for (size_t i = 0; i < errorNumbers.size(); i++)
			errorPages.insert(std::make_pair(errorNumbers[i], element));	
	}
	// Size restrictions
	else if (key == CLIMAXBODY)
		clientMaxBody = value;
	else if (key == MAXCONNS)
		maxConnections = value;
	else if (key == BACKLOG)
		backlog = value;
	else
		std::cerr << I_INVALIDKEY << key << "'. Connected value: '" << value << "'." << std::endl;
}
*/

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
	std::cout << "Info: getConfigs: " << configList.size() << (configList.size() == 1 ? " config element built." : " config elements built.") << std::endl;
	if (configList.empty())
		throw std::runtime_error(E_NOSERVER);
	return configList;
}

std::string configFileToString(const char* path)
{
	std::ifstream		infile(path);
	std::string			line;
	std::stringstream	buffer;
	
	if (!infile)
	{
		std::string invalidpath(path);
		throw std::runtime_error(E_FILEOPEN + invalidpath + '\n');
	}
	while (std::getline(infile, line))
	{
		if (line[line.find_first_not_of(WHITESPACE)] != '#')
			buffer << line;
	}
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
		throw std::runtime_error(E_ELMNTDECL + elementTitle + '\n');
	len_close = configString.find("}");
	if (configString.find("{") < len_close)
		throw std::runtime_error(E_SUBELEMNT + configString + '\n');
	elementBody = splitEraseStr(configString, "}");
	return trim(elementBody);
}
