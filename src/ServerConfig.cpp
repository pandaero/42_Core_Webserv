#include "../include/ServerConfig.hpp"

ServerConfig::ServerConfig(std::string configStr, strMap* mimeTypes)
{
	std::string	instruction, key;
	
	_mimeTypes = mimeTypes;

	while (!configStr.empty())
	{
		instruction = getInstruction(configStr);
		key = splitEraseTrimChars(instruction, " \t\v\r\n{");
		
		// vars directly in server block
		if (key == SERVERNAME)
			parseNames(instruction);
		else if (key == HOST)
			parseHost(instruction);
		else if (key == PORT)
			parsePort(instruction);
		else if (key == ROOT)
			parseRoot(instruction);
		else if (key == DIRLISTING)
			parseDirListing(instruction);
		else if (key == CLIMAXBODY)
			parseClientMaxBody(instruction);
		else if (key == MAXCONNS)
			parseMaxConnections(instruction);
		else if (key == STDFILE)
			parseStandardFile(instruction);
		
		// subelement blocks
		else if (key == ERRORPAGETITLE)
			parseStatusPagePaths(instruction);
		else if (key == LOCATIONTITLE)
			parseLocations(instruction);
		else if (key == CGITITLE)
			parseCgiPaths(instruction);
		else
			std::cerr << I_SC_INVALIDKEY << key << std::endl;
	}
}

strVec ServerConfig::getNames() const { return _names; }

in_addr_t ServerConfig::getHost() const { return _host; }

in_port_t ServerConfig::getPort() const { return _port; }

std::string ServerConfig::getRoot() const { return _root; }

bool ServerConfig::getDefaultDirlisting() const { return _defaultDirListing; }

uint64_t ServerConfig::getClientMaxBody() const { return _clientMaxBody; }

size_t ServerConfig::getMaxConnections() const { return _maxConnections; }

std::string ServerConfig::getStandardFile() const { return _standardFile; }

intStrMap ServerConfig::getStatusPagePaths() const { return _statusPagePaths; }

strLocMap ServerConfig::getLocations() const { return _locations; }

strMap ServerConfig::getCgiPaths() const { return _cgiPaths; }

strMap*	ServerConfig::getMIMETypes() const { return _mimeTypes; }

std::vector<ServerConfig> ServerConfig::getAltConfigs() const { return _altConfigs; }

void ServerConfig::parseNames(std::string& input)
{
	std::string name;

	while (!input.empty())
	{
		name = splitEraseTrimChars(input, WHITESPACE);
		if (isAlnumStrPlus(name, "._-/"))
			_names.push_back(name);
		else
			std::cerr << I_SC_INVALSERVNAME << name << std::endl;
	}
	if (_names.empty())
		_names.push_back("default");
}

void ServerConfig::parseHost(std::string& input)
{
	if (input == "ANY")
		_host = INADDR_ANY;
	else
	{
		_host = inet_addr(input.c_str());
		if (_host == INADDR_NONE)
			throw std::runtime_error(E_SC_HOSTADDRVAL + input + '\n');
	}
}

void ServerConfig::parsePort(std::string& input)
{
	if (input.find_first_not_of("0123456789") != std::string::npos)
		throw std::runtime_error(E_SC_PORTINPUT + input + '\n');
	_port = (uint16_t)atoi(input.c_str());
	if (_port > (uint16_t)65534)
		throw std::runtime_error(E_SC_PORTVAL + input + '\n');
	_port = htons(_port);

}

void ServerConfig::parseRoot(std::string& input)
{
	if (!isAlnumStrPlus(input, "._-/"))
		throw std::runtime_error(E_SC_ROOTINPUT + input + '\n');
	_root = input;

	if (*(_root.end() - 1) != '/')
		std::cerr << I_SC_NONDIRPATH << _root << std::endl;
}

void ServerConfig::parseDirListing(std::string& input)
{
	if (input == "yes")
		_defaultDirListing = true;
	else
	{
		_defaultDirListing = false;
		if (input != "no")
			std::cerr << I_SC_INVALIDVALUE << input << std::endl;
	}
}

void ServerConfig::parseClientMaxBody(std::string& input)
{
	if (input.find_first_not_of("0123456789") != std::string::npos)
			throw std::runtime_error(E_SC_MAXCLBODINPUT + input + '\n');
	std::istringstream iss(input);
	iss >> _clientMaxBody;
	if (_clientMaxBody > (uint64_t)MAX_MAXCLIENTBODY)
		throw std::runtime_error(E_SC_MAXCLBODHIGH + input + '\n');
}

void ServerConfig::parseMaxConnections(std::string& input)
{
	if (input.find_first_not_of("0123456789") != std::string::npos)
			throw std::runtime_error(E_SC_MAXCONNINPUT + input + '\n');
	_maxConnections = atoi(input.c_str());
	if (_maxConnections > MAX_MAXCONNECTIONS)
		throw std::runtime_error(E_SC_MAXCONNVAL + input + '\n');
}

void ServerConfig::parseStandardFile(std::string& input)
{
	if (!isAlnumStrPlus(input, "._-/"))
		throw std::runtime_error(E_SC_STDFILEINPUT + input + '\n');
	_standardFile = input;
}

void ServerConfig::parseStatusPagePaths(std::string& userErrorPages)
{
	std::string		key, value;
	strVec			lineStrings;
	int				code;
	
	while (!userErrorPages.empty())
	{
		lineStrings = splitEraseStrVec(userErrorPages, WHITESPACE, ";");
		if (lineStrings.size() < 2)
		{
			std::cerr << I_SC_MISSINGVAL << lineStrings[0] << std::endl;
			continue;
		}
		value = lineStrings.back();
		lineStrings.pop_back();
		while (!lineStrings.empty())
		{
			key = lineStrings.back();
			code = atoi(key.c_str());
			if (_statusPagePaths.find(code) != _statusPagePaths.end())
				_statusPagePaths[code] = value;
			else if (code > 99 && code < 600)
				_statusPagePaths.insert(std::make_pair(code, value));
			else
				std::cerr << I_SC_INVALSTATPAGE << key << std::endl;
			lineStrings.pop_back();
		}
	}
}

void ServerConfig::parseLocations(std::string& locationElement)
{
	std::string		path, instruction, key;
	strLocMap_it	iter;
	s_locInfo		locInfo;
	
	path = splitEraseTrimChars(locationElement, WHITESPACE);
	if (*(path.end() - 1) != '/')
			std::cerr << I_SC_NONDIRPATH << path << std::endl;

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
				std::cerr << I_SC_INVALIDVALUE << instruction << std::endl;
		}
		else if (key == REDIRECTION)
			locInfo.http_redir = instruction;
		else if (key == UPLOADDIR)
			locInfo.upload_dir = instruction;
		else if (key == STDFILE)
			locInfo.std_file = instruction;
		else
			std::cerr << I_SC_INVALIDKEY << key << std::endl;
	}
	_locations.insert(std::make_pair(path, locInfo));
}

void ServerConfig::parseCgiPaths(std::string& userCgiElement)
{
	std::string	instruction, key;
	strMap_it	iter;
	
	while (!userCgiElement.empty())
	{
		instruction = getInstruction(userCgiElement);
		key = splitEraseTrimChars(instruction, WHITESPACE);
		if (instruction.empty())
		{
			std::cerr << I_SC_MISSINGVAL << key << std::endl;
			continue;
		}
		iter = _cgiPaths.find(key);
		if (iter != _cgiPaths.end())
			iter->second = instruction;
		else
			_cgiPaths.insert(std::make_pair(key, instruction));
	}
}

void ServerConfig::addAltConfig(const ServerConfig& altConfig)
{
	_altConfigs.push_back(altConfig);
}
