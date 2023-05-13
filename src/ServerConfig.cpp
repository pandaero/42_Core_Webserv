#include "../include/ServerConfig.hpp"

ServerConfig::ServerConfig() {}

/*
This constructor is only called when reading the default config.
The key values contained in the passed string will all be accepted,
forming the template to check against when parsing user supplied config
files.
*/
ServerConfig::ServerConfig(std::string defaultConfigStr)
{
	std::string	instruction, key, value;
	
	while (!defaultConfigStr.empty())
	{
		instruction = getInstruction(defaultConfigStr);
		key = splitEraseChars(instruction, " \t\v\r\n{");
		if (key == ERRORPAGETITLE)
			parseDefaultErrorPages(instruction);
		else if (key == LOCATIONTITLE)
			parseLocation(instruction);
		else if (key == CGITITLE)
			parseDefaultCgi(instruction);
		else 
			_configPairs.insert(make_pair(key, instruction));
	}
	whoIsI();
}

ServerConfig::ServerConfig(const ServerConfig& src)
{
	*this = src;
}

ServerConfig::~ServerConfig() {}

ServerConfig& ServerConfig::operator=(const ServerConfig& src)
{
	_configPairs = src._configPairs;
	_errorPages = src._errorPages;
	_locations = src._locations;
	_cgiPaths = src._cgiPaths;
	return *this;
}

void ServerConfig::applySettings(std::string userConfigStr)
{
	std::string	key, value;
	strMap_it	iter;
	
	while (!trim(userConfigStr).empty())
	{
		key = splitEraseChars(userConfigStr, WHITESPACE);
		trim(key);
		trim(userConfigStr);
		if (key == ERRORPAGETITLE)
			parseUserErrorPages(userConfigStr);
		else if (key == LOCATIONTITLE)
			parseLocation(userConfigStr);
		else if (key == CGITITLE)
			parseUserCgi(userConfigStr);
		else 
		{
			value = splitEraseChars(userConfigStr, ";");
			iter = _configPairs.find(key);
			if (iter != _configPairs.end())
				iter->second = value;
			else
				std::cerr << I_INVALIDKEY << key << std::endl;
		}
	}
}

void ServerConfig::whoIsI()
{
	std::cout << "*****_configPairs*****\n";
	for (strMap_it it = _configPairs.begin(); it != _configPairs.end(); it++)
		std::cout << it->first << "\t\t" << it->second << std::endl;
	
	
	std::cout << "*****_errorPages*****\n";
	for (intStrMap_it it = _errorPages.begin(); it != _errorPages.end(); it++)
		std::cout << it->first << "\t\t" << it->second << std::endl;

	std::cout << "*****_locations*****\n";
	for (strLocMap_it it = _locations.begin(); it != _locations.end(); it++)
		std::cout << it->first << "\t\t" << "methods: " << (it->second.get == true ? "get" : "") << (it->second.post == true ? " post" : "") << (it->second.delete_ == true ? " delete" : "") << ". alt_location: " << it->second.alt_location << ". dir_listing: " << it->second.dir_listing << std::endl;
	
	std::cout << "*****_cgiPaths*****\n";
	for (strMap_it it = _cgiPaths.begin(); it != _cgiPaths.end(); it++)
		std::cout << it->first << "\t\t" << it->second  << std::endl;
}

strMap ServerConfig::getConfigPairs() const
{
	return _configPairs;
}

intStrMap ServerConfig::getErrorPaths() const
{
	return _errorPages;
}

strLocMap ServerConfig::getLocations() const
{
	return _locations;
}

strMap	ServerConfig::getCgiPaths() const
{
	return _cgiPaths;
}

/*
Should the user be able to select new error codes and assign their values?
This is not the function that does that (this is the default parser), but
wanted to add the note here to not forget it.
*/
void ServerConfig::parseDefaultErrorPages(std::string& defaultErrorPages)
{
	std::string	key, value;
	strVec		lineStrings;

	while (!defaultErrorPages.empty())
	{
		lineStrings = splitEraseStrVec(defaultErrorPages, WHITESPACE, ";");
		value = lineStrings.back();
		lineStrings.pop_back();
		while (!lineStrings.empty())
		{
			key = lineStrings.back();
			if (key == "default")
				_errorPages.insert(std::make_pair(-1, value));
			else
				_errorPages.insert(std::make_pair(atoi(key.c_str()), value));
			lineStrings.pop_back();
		}
	}	
}

void ServerConfig::parseUserErrorPages(std::string& userErrorPages)
{
	std::string		key, value;
	int				code;
	intStrMap_it	iter;
	
	while (!userErrorPages.empty())
	{
		key = splitEraseChars(userErrorPages, WHITESPACE);
		code = atoi(key.c_str());
		value = splitEraseChars(userErrorPages, ";");
		iter = _errorPages.find(code);
		if (iter != _errorPages.end())
			iter->second = value;
		else
			std::cerr << I_INVALIDKEY << key << std::endl;
	}
}

/*
Had to abandon the elegant "take what you find in the default and put it into the map" style
because the location element is structured differently: it has an extra identifier, the path.
Made a struct to gather the info and we still end up with just a map.

This rigid structure has one positive: the exact same function can be used to parse the default
and the user config file (because the function performs input checking).
*/
void ServerConfig::parseLocation(std::string& locationElement)
{
	std::string		path, instruction, key;
	strLocMap_it	iter;
	s_locInfo		locInfo;
	
	path = splitEraseChars(locationElement, WHITESPACE);
	while (!locationElement.empty())
	{
		instruction = getInstruction(locationElement);
		key = splitEraseChars(instruction, WHITESPACE);
		if (key == METHODS)
		{
			if (instruction.find(GET) != std::string::npos)
				locInfo.get = true;
			else
				locInfo.get = false;
			if (instruction.find(POST) != std::string::npos)
				locInfo.post = true;
			else
				locInfo.post = false;
			if (instruction.find(DELETE) != std::string::npos)
				locInfo.delete_ = true;
			else
				locInfo.delete_ = false;
		}
		else if (key == DIRLISTING)
			locInfo.dir_listing = instruction;
		else if (key == ALTLOC)
			locInfo.alt_location = instruction;
		else
			std::cerr << I_INVALIDKEY << key << std::endl;
	}
	_locations.insert(std::make_pair(path, locInfo));
}

void ServerConfig::parseDefaultCgi(std::string& defaultCgiElement)
{
	std::string	instruction, key;
	
	while (!defaultCgiElement.empty())
	{
		instruction = getInstruction(defaultCgiElement);
		key = splitEraseChars(instruction, WHITESPACE);
		_cgiPaths.insert(std::make_pair(key, instruction));
	}
}

void ServerConfig::parseUserCgi(std::string& userCgi)
{
	std::string	key, value;
	strMap_it	iter;
	
	while (!userCgi.empty())
	{
		key = splitEraseChars(userCgi, WHITESPACE);
		value = splitEraseChars(userCgi, ";");
		iter = _cgiPaths.find(key);
		if (iter != _cgiPaths.end())
			iter->second = value;
		else
			std::cerr << I_INVALIDKEY << key << std::endl;
	}
}

/* std::string ServerConfig::getSubElement(std::string& configStr)
{
	size_t			len_close;
	std::string		elementBody;
	
	trim(configStr);
	len_close = configStr.find("}");
	if (configStr.find("{") < len_close)
		throw std::runtime_error(E_SUBELEMNT + configStr);
	elementBody = splitEraseChars(configStr, "}");
	return trim(elementBody);
} */

/* std::string	ServerConfig::getInstruction(std::string& configStr)
{
	size_t		len_open, len_close;
	
	len_open = configStr.find("{");
	if (configStr.find(";") < len_open)
		return splitEraseChars(configStr, ";");
	len_close = configStr.find("}");
	if (configStr.find("{", len_open + 1) < len_close)
		throw std::runtime_error(E_SUBELEMNT + configStr);
	return splitEraseChars(configStr, "}");
} */
