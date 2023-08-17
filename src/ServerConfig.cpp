#include "../include/ServerConfig.hpp"

ServerConfig::ServerConfig()
{}

/*
This constructor is only called when reading the default config.
The key values contained in the passed string will all be accepted,
forming the template to check against when parsing user supplied config
files.
Note: parseLocation has only one version for default and client parsing
because it performs its own and rigid input checks.
*/
ServerConfig::ServerConfig(std::string defaultConfigStr, strMap* mimeTypes)
{
	std::string	instruction, key;
	
	while (!defaultConfigStr.empty())
	{
		instruction = getInstruction(defaultConfigStr);
		key = splitEraseTrimChars(instruction, " \t\v\r\n{");
		if (key == ERRORPAGETITLE)
			parseDefaultErrorPages(instruction);
		else if (key == LOCATIONTITLE)
			parseLocation(instruction);
		else if (key == CGITITLE)
			parseDefaultCgi(instruction);
		else 
			_configPairs.insert(make_pair(key, instruction));
	}
	_mimeTypes = mimeTypes;
	_sharedNetAddr = false;
}

ServerConfig::ServerConfig(const ServerConfig& src)
{
	*this = src;
}

ServerConfig& ServerConfig::operator=(const ServerConfig& src)
{
	_configPairs = src._configPairs;
	_errorPages = src._errorPages;
	_locations = src._locations;
	_cgiPaths = src._cgiPaths;
	_mimeTypes = src._mimeTypes;
	_sharedNetAddr = src._sharedNetAddr;
	return *this;
}

void ServerConfig::applySettings(std::string userConfigStr)
{
	std::string	instruction, key;
	strMap_it	iter;
	
	while (!userConfigStr.empty())
	{
		instruction = getInstruction(userConfigStr);
		key = splitEraseTrimChars(instruction, " \t\v\r\n{");
		if (key == ERRORPAGETITLE)
			parseUserErrorPages(instruction);
		else if (key == LOCATIONTITLE)
			parseLocation(instruction);
		else if (key == CGITITLE)
			parseUserCgi(instruction);
		else 
		{
			iter = _configPairs.find(key);
			if (iter != _configPairs.end())
				iter->second = instruction;
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
		std::cout << it->first << "\t\t" << "methods: " << (it->second.get == true ? "get" : "") << (it->second.post == true ? " post" : "") << (it->second.delete_ == true ? " delete" : "") << ". alt_location: " << it->second.http_redir << ". dir_listing: " << it->second.dir_listing << std::endl;
	
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

strMap ServerConfig::getCgiPaths() const
{
	return _cgiPaths;
}

strMap*	ServerConfig::getMIMETypes() const
{
	return _mimeTypes;
}

bool ServerConfig::getSharedNetAddr() const
{
	return _sharedNetAddr;
}

void ServerConfig::setSharedNetAddr(bool value)
{
	_sharedNetAddr = value;
}

void ServerConfig::parseDefaultErrorPages(std::string& defaultErrorPages)
{
	std::string	key, value;
	strVec		lineStrings;

	while (!defaultErrorPages.empty())
	{
		lineStrings = splitEraseStrVec(defaultErrorPages, WHITESPACE, ";");
		if (lineStrings.size() < 2)
		{
			std::cerr << I_INVALERRPAGE << std::endl;
			continue;
		}
		value = lineStrings.back();
		lineStrings.pop_back();
		while (!lineStrings.empty())
		{
			key = lineStrings.back();
			lineStrings.pop_back();
			_errorPages.insert(std::make_pair(atoi(key.c_str()), value));	
		}
	}	
}

void ServerConfig::parseUserErrorPages(std::string& userErrorPages)
{
	std::string		key, value;
	strVec			lineStrings;
	int				code;
	
	while (!userErrorPages.empty())
	{
		lineStrings = splitEraseStrVec(userErrorPages, WHITESPACE, ";");
		value = lineStrings.back();
		lineStrings.pop_back();
		while (!lineStrings.empty())
		{
			key = lineStrings.back();
			code = atoi(key.c_str());
			if (_errorPages.find(code) != _errorPages.end())
				_errorPages[code] = value;
			else if (code > 99 && code < 600)
				_errorPages.insert(std::make_pair(code, value));
			else
				std::cerr << I_INVALIDKEY << key << std::endl;
			lineStrings.pop_back();
		}
	}
}

void ServerConfig::parseLocation(std::string& locationElement)
{
	std::string		path, instruction, key;
	strLocMap_it	iter;
	s_locInfo		locInfo;
	
	path = splitEraseTrimChars(locationElement, WHITESPACE);
	while (!locationElement.empty())
	{
		instruction = getInstruction(locationElement);
		key = splitEraseTrimChars(instruction, WHITESPACE);
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
		{
			if (instruction == "yes" || instruction == "no")
				locInfo.dir_listing = instruction;
			else
				std::cerr << I_INVALIDVALUE << instruction << std::endl;
		}
		else if (key == REDIRECTION)
			locInfo.http_redir = instruction;
		else if (key == UPLOADDIR)
			locInfo.upload_dir = instruction;
		else if (key == STDFILE)
			locInfo.std_file = instruction;
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
		key = splitEraseTrimChars(instruction, WHITESPACE);
		_cgiPaths.insert(std::make_pair(key, instruction));
	}
}

void ServerConfig::parseUserCgi(std::string& userCgiElement)
{
	std::string	instruction, key;
	strMap_it	iter;
	
	while (!userCgiElement.empty())
	{
		instruction = getInstruction(userCgiElement);
		key = splitEraseTrimChars(instruction, WHITESPACE);
		iter = _cgiPaths.find(key);
		if (iter != _cgiPaths.end())
			iter->second = instruction;
		else
			std::cerr << I_INVALIDKEY << key << std::endl;
	}
}
