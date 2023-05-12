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

strMap ServerConfig::getErrorPaths() const
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

Removed the ability to assign multiple codes to one page.
Can reimplement without much work, but it would add weight to the function
and not really serve an important purpose.
*/
void ServerConfig::parseDefaultErrorPages(std::string& defaultConfigStr)
{
	std::string	subelement, key, value;
	
	subelement = getSubElement(defaultConfigStr);
	while (!trim(subelement).empty())
	{
		key = splitEraseChars(subelement, WHITESPACE);
		trim(subelement);
		value = splitEraseChars(subelement, ";");
		_errorPages.insert(std::make_pair(key, value));
	}
}

void ServerConfig::parseUserErrorPages(std::string& userConfigStr)
{
	std::string	subelement, key, value;
	strMap_it	iter;
	
	subelement = getSubElement(userConfigStr);
	while (!trim(subelement).empty())
	{
		key = splitEraseChars(subelement, WHITESPACE);
		trim(subelement);
		value = splitEraseChars(subelement, ";");
		iter = _errorPages.find(key);
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
	std::string		path, subelement, key, value;
	strLocMap_it	iter;
	s_locInfo		locInfo;
	
	path = splitEraseChars(defaultConfigStr, " ");
	subelement = getSubElement(defaultConfigStr);
	while (!trim(subelement).empty())
	{
		key = splitEraseChars(subelement, WHITESPACE);
		trim(subelement);
		value = splitEraseChars(subelement, ";");
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
	std::string	subelement, key, value;
	
	subelement = getSubElement(defaultConfigStr);
	while (!trim(subelement).empty())
	{
		key = splitEraseChars(subelement, WHITESPACE);
		trim(subelement);
		value = splitEraseChars(subelement, ";");
		_cgiPaths.insert(std::make_pair(key, value));
	}
}

void ServerConfig::parseUserCgi(std::string& userConfigStr)
{
	std::string	subelement, key, value;
	strMap_it	iter;
	
	subelement = getSubElement(userConfigStr);
	while (!trim(subelement).empty())
	{
		key = splitEraseChars(subelement, WHITESPACE);
		trim(subelement);
		value = splitEraseChars(subelement, ";");
		iter = _cgiPaths.find(key);
		if (iter != _errorPages.end())
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


/*
void ServerConfig::setField(std::string key, std::string value)
{


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
}
*/
