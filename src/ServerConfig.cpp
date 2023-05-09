#include "../include/ServerConfig.hpp"

ServerConfig::ServerConfig(std::string serverConfigStr)
{
	// Parse default config
	std::string	defaultConfigData, defaultConfigElement;
	std::string	key, value;
	
	// defaultConfigData = configFileToString(DEFAULTCONFIGPATH);
	// defaultConfigElement = getConfigElement(defaultConfigData);
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



/*
For now, only accepts "server" elements. Probably sufficient for the subject.
But quickly expandable if we have to add other elements.

Does not allow declaration of subelements at this point.
*/
#define SERVER	"server"
// remove define for subelement not allowed
typedef enum
{
	server,
	error_pages,
	location
} parse_mode;
