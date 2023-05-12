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
	std::string	key, value;
	
	while (!trim(defaultConfigStr).empty())
	{
		key = splitEraseChars(defaultConfigStr, WHITESPACE);
		trim(defaultConfigStr);
		if (key == ERRORPAGETITLE)
			parseDefaultErrorPages(defaultConfigStr);
		else if (key == LOCATIONTITLE)
			parseLocation(defaultConfigStr);
		else if (key == CGITITLE)
			parseDefaultCgi(defaultConfigStr);
		else 
		{
			value = splitEraseChars(defaultConfigStr, ";");
			_configPairs.insert(make_pair(key, value));
		}
	}
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
void ServerConfig::parseDefaultErrorPages(std::string& defaultConfigStr)
{
	std::string	elementBody, key, value;
	strVec		lineStrings;

	elementBody = getSubElement(defaultConfigStr);
	while (!trim(elementBody).empty())
	{
		lineStrings = splitEraseStrVec(elementBody, WHITESPACE, ";");
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

void ServerConfig::parseUserErrorPages(std::string& userConfigStr)
{
	std::string		elementBody, key, value;
	int				code;
	intStrMap_it	iter;
	
	elementBody = getSubElement(userConfigStr);
	while (!trim(elementBody).empty())
	{
		key = splitEraseChars(elementBody, WHITESPACE);
		code = atoi(key.c_str());
		trim(elementBody);
		value = splitEraseChars(elementBody, ";");
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
void ServerConfig::parseLocation(std::string& defaultConfigStr)
{
	std::string		path, elementBody, key, value;
	strLocMap_it	iter;
	s_locInfo		locInfo;
	
	path = splitEraseChars(defaultConfigStr, " ");
	elementBody = getSubElement(defaultConfigStr);
	while (!trim(elementBody).empty())
	{
		key = splitEraseChars(elementBody, WHITESPACE);
		trim(elementBody);
		value = splitEraseChars(elementBody, ";");
		if (key == METHODS)
		{
			if (value.find(GET) != std::string::npos)
				locInfo.get = true;
			else
				locInfo.get = false;
			if (value.find(POST) != std::string::npos)
				locInfo.get = true;
			else
				locInfo.get = false;
			if (value.find(DELETE) != std::string::npos)
				locInfo.delete_ = true;
			else
				locInfo.delete_ = false;
		}
		else if (key == DIRLISTING)
			locInfo.dir_listing = value;
		else if (key == ALTLOC)
			locInfo.alt_location = value;
		else
			std::cerr << I_INVALIDKEY << key << std::endl;
	}
	_locations.insert(std::make_pair(path, locInfo));
}

void ServerConfig::parseDefaultCgi(std::string& defaultConfigStr)
{
	std::string	elementBody, key, value;
	
	elementBody = getSubElement(defaultConfigStr);
	while (!trim(elementBody).empty())
	{
		key = splitEraseChars(elementBody, WHITESPACE);
		trim(elementBody);
		value = splitEraseChars(elementBody, ";");
		_cgiPaths.insert(std::make_pair(key, value));
	}
}

void ServerConfig::parseUserCgi(std::string& userConfigStr)
{
	std::string	elementBody, key, value;
	strMap_it	iter;
	
	elementBody = getSubElement(userConfigStr);
	while (!trim(elementBody).empty())
	{
		key = splitEraseChars(elementBody, WHITESPACE);
		trim(elementBody);
		value = splitEraseChars(elementBody, ";");
		iter = _cgiPaths.find(key);
		if (iter != _cgiPaths.end())
			iter->second = value;
		else
			std::cerr << I_INVALIDKEY << key << std::endl;
	}
}

std::string ServerConfig::getSubElement(std::string& configStr)
{
	size_t			len_close;
	std::string		elementBody;
	
	len_close = configStr.find("}");
	if (configStr.find("{", 1) < len_close)
		throw std::runtime_error(E_SUBELEMNT + configStr);
	elementBody = splitEraseChars(configStr, "}");
	return trim(elementBody);
}
