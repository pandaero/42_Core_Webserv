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
			parseErrorPages(defaultConfigStr);
		else if (key == LOCATIONTITLE)
			parseLocation(defaultConfigStr);
		else if (key == CGITITLE)
			parseCgi(defaultConfigStr);
		else 
		{
			value = splitEraseChars(defaultConfigStr, ";");
			_errorPages.insert(make_pair(key, value));
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
	return *this;
}

strMap ServerConfig::getConfigPairs() const
{
	return _configPairs;
}

strMap ServerConfig::getConfigErrorPaths() const
{
	return _errorPages;
}

/*
Should the user be able to select new error codes and assign their values?
This is not the function that does that (this is the default parser), but
wanted to add the note here to not forget it.

Removed the ability to assign multiple codes to one page.
Can reimplement without much work, but it would add weight to the function
and not really serve an important purpose.
*/
void ServerConfig::parseErrorPages(std::string& serverConfigStr)
{
	std::string	subelement, key, value;
	strMap_it	iter;
	
	subelement = getSubElement(serverConfigStr);
	while (!trim(subelement).empty())
	{
		key = splitEraseChars(subelement, WHITESPACE);
		trim(subelement);
		value = splitEraseChars(subelement, ";");
		_errorPages.insert(std::make_pair(key, value));
	}
}

void ServerConfig::parseLocation(std::string& serverConfigStr)
{
	std::string		path, subelement, key, value;
	strLocMap_it	iter;
	
	path = splitEraseChars(serverConfigStr, " ");
	subelement = getSubElement(serverConfigStr);
	while (!trim(subelement).empty())
	{
		s_locInfo	locInfo;

		key = splitEraseChars(subelement, WHITESPACE);
		trim(subelement);
		value = splitEraseChars(subelement, ";");
		_locations.insert(std::make_pair(key, locInfo));

		


	}
}

std::string ServerConfig::getSubElement(std::string& serverConfigStr)
{
	size_t			len_close;
	std::string		elementBody;
	
	len_close = serverConfigStr.find("}");
	if (serverConfigStr.find("{") < len_close)
		throw std::runtime_error(E_SUBELEMNT + serverConfigStr);
	elementBody = splitEraseChars(serverConfigStr, "}");
	return trim(elementBody);
}

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
// remove define for subelement not allowed
typedef enum
{
	server,
	error_pages,
	location
} parse_mode;
