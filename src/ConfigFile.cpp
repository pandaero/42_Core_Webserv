#include "../include/ConfigFile.hpp"

ConfigFile::ConfigFile(const char* userConfigPath)
{
	std::string	configData;
	std::string defaultFile = "system/default.conf";
	
	// All "servers" receive a pointer to this string map to select MIME types.
	setMIMEtypes();
	
	// Parse default ServerConfig object from internal default config file.
	// This default config file is effectively part of the code:
	// The key value pairs in it will all be accepted; they define the 
	// framework for the parsing of the user supplied config files.
	_defaultServerConfig = NULL;
	configData = loadFile(defaultFile.c_str());
	_defaultServerConfig = new ServerConfig(getServerConfigElement(configData), &_mimeTypes);
	std::cout << I_DEFAULTIMPORT << defaultFile << std::endl;
	
	// parse intended ServerConfig objects from client supplied config file
	configData = loadFile(userConfigPath);
	while (!configData.empty())
	{
		ServerConfig newConfig(*_defaultServerConfig);
		newConfig.applySettings(getServerConfigElement(configData));
		_serverConfigs.push_back(newConfig);
	}
	if (_serverConfigs.empty())
		throw std::runtime_error(E_NOSERVER);
	if (_serverConfigs.size() > 10)
		throw std::runtime_error(E_MANYSERVER);
	
	// check for multiple "servers" on the same host:port combination
	for (size_t i = 0; i < _serverConfigs.size(); ++i)
	{
		if (combineSharedNetAddr(_serverConfigs[i], i))
			_serverConfigs.erase(_serverConfigs.begin() + i--);
	}
	// need another loop to add to _servers, because the ServerConfig has to be finalized first 
	for (size_t i = 0; i < _serverConfigs.size(); ++i)
		_servers.push_back(Server(_serverConfigs[i]));
	std::cout << I_CONFIGIMPORT << std::endl;
}

ConfigFile::~ConfigFile()
{
	if (_defaultServerConfig)
		delete _defaultServerConfig;
}

std::vector<ServerConfig> ConfigFile::getConfigs() const
{
	return _serverConfigs;
}

std::vector<Server>	ConfigFile::getServers() const
{
	return _servers;
}

std::string ConfigFile::loadFile(const char* path)
{
	std::ifstream		infile(path);
	std::stringstream	buffer;
	std::string			line;
	
	if (!infile)
	{
		std::string invalidpath(path);
		throw std::runtime_error(E_FILEOPEN + invalidpath + '\n');
	}
	while (std::getline(infile, line))
	{
		if (line[line.find_first_not_of(WHITESPACE)] == '#')
			continue;
		buffer << line;
	}
	infile.close();
	line = buffer.str();
	trim(line);
	return line;
}

std::string ConfigFile::getServerConfigElement(std::string& configData)
{
	std::string	elementTitle;
	
	elementTitle = splitEraseTrimChars(configData, "{");
	if (elementTitle != SERVER)
		throw std::runtime_error(E_ELMNTDECL + elementTitle + '\n');
	return getInstruction(configData);
}

bool ConfigFile::combineSharedNetAddr(const ServerConfig& currentConfig, size_t currentConfigIndex)
{
	for (size_t i = 0; i < currentConfigIndex; ++i)
	{
		if (sharedNetAddr(currentConfig, _serverConfigs[i]))
		{
			std::cout << "Shared netAddr detected." << std::endl;
			std::cout << "Adding current ServerConfig to ServerConfig index " << i << std::endl;
			_serverConfigs[i].addAltConfig(currentConfig);
			return true;
		}
	}
	return false;
}

bool ConfigFile::sharedNetAddr(const ServerConfig& a, const ServerConfig& b)
{
	if (&a == &b)
		return false;
	if (a.getConfigPairs()[HOST] == b.getConfigPairs()[HOST]
		&& a.getConfigPairs()[PORT] == b.getConfigPairs()[PORT])
		return true;
	return false;
}



void ConfigFile::setMIMEtypes()
{
	_mimeTypes[".html"] = "text/html";
	_mimeTypes[".htm"] = "text/html";
	_mimeTypes[".css"] = "text/css";
	_mimeTypes[".js"] = "application/javascript";
	_mimeTypes[".json"] = "application/json";
	_mimeTypes[".jpg"] = "image/jpeg";
	_mimeTypes[".jpeg"] = "image/jpeg";
	_mimeTypes[".png"] = "image/png";
	_mimeTypes[".gif"] = "image/gif";
	_mimeTypes[".bmp"] = "image/bmp";
	_mimeTypes[".ico"] = "image/x-icon";
	_mimeTypes[".svg"] = "image/svg+xml";
	_mimeTypes[".xml"] = "application/xml";
	_mimeTypes[".pdf"] = "application/pdf";
	_mimeTypes[".zip"] = "application/zip";
	_mimeTypes[".gz"] = "application/gzip";
	_mimeTypes[".tar"] = "application/x-tar";
	_mimeTypes[".mp4"] = "video/mp4";
	_mimeTypes[".mpeg"] = "video/mpeg";
	_mimeTypes[".avi"] = "video/x-msvideo";
	_mimeTypes[".avif"] = "image/avif";
	_mimeTypes[".ogg"] = "audio/ogg";
	_mimeTypes[".mp3"] = "audio/mpeg";
	_mimeTypes[".wav"] = "audio/wav";
	_mimeTypes[".mov"] = "video/quicktime";
	_mimeTypes[".ppt"] = "application/vnd.ms-powerpoint";
	_mimeTypes[".xls"] = "application/vnd.ms-excel";
	_mimeTypes[".doc"] = "application/msword";
	_mimeTypes[".csv"] = "text/csv";
	_mimeTypes[".txt"] = "text/plain";
	_mimeTypes[".rtf"] = "application/rtf";
	_mimeTypes[".shtml"] = "text/html";
	_mimeTypes[".php"] = "application/php";
	_mimeTypes[".jsp"] = "text/plain";
	_mimeTypes[".swf"] = "application/x-shockwave-flash";
	_mimeTypes[".ttf"] = "application/x-font-truetype";
	_mimeTypes[".eot"] = "application/vnd.ms-fontobject";
	_mimeTypes[".woff"] = "application/font-woff";
	_mimeTypes[".woff2"] = "font/woff2";
	_mimeTypes[".ics"] = "text/calendar";
	_mimeTypes[".vcf"] = "text/x-vcard";
	_mimeTypes[".mid"] = "audio/midi";
	_mimeTypes[".midi"] = "audio/midi";
	_mimeTypes[".wmv"] = "video/x-ms-wmv";
	_mimeTypes[".webm"] = "video/webm";
	_mimeTypes[".3gp"] = "video/3gpp";
	_mimeTypes[".3g2"] = "video/3gpp2";
	_mimeTypes[".pl"] = "text/plain";
	_mimeTypes[".py"] = "text/x-python";
	_mimeTypes[".java"] = "text/x-java-source";
	_mimeTypes[".c"] = "text/x-c";
	_mimeTypes[".cpp"] = "text/x-c++";
	_mimeTypes[".cs"] = "text/plain";
	_mimeTypes[".rb"] = "text/x-ruby";
	_mimeTypes[".htm"] = "text/html";
	_mimeTypes[".shtml"] = "text/html";
	_mimeTypes[".xhtml"] = "application/xhtml+xml";
	_mimeTypes[".m4a"] = "audio/mp4";
	_mimeTypes[".mp4a"] = "audio/mp4";
	_mimeTypes[".oga"] = "audio/ogg";
	_mimeTypes[".ogv"] = "video/ogg";
	_mimeTypes[".ogx"] = "application/ogg";
	_mimeTypes[".oga"] = "audio/ogg";
	_mimeTypes[".m3u8"] = "application/vnd.apple.mpegurl";
	_mimeTypes[".qt"] = "video/quicktime";
	_mimeTypes[".ts"] = "video/mp2t";
	_mimeTypes[".xl"] = "application/excel";
	_mimeTypes[".cab"] = "application/vnd.ms-cab-compressed";
	_mimeTypes[".msi"] = "application/x-msdownload";
	_mimeTypes[".dmg"] = "application/x-apple-diskimage";
	_mimeTypes[".exe"] = "application/octet-stream";
	_mimeTypes[".bin"] = "application/octet-stream";
	_mimeTypes[".ps"] = "application/postscript";
	_mimeTypes[".so"] = "application/octet-stream";
	_mimeTypes[".dll"] = "application/octet-stream";
	_mimeTypes[".m4v"] = "video/x-m4v";
	_mimeTypes[".ser"] = "application/java-serialized-object";
	_mimeTypes[".sh"] = "application/x-sh";
	_mimeTypes[".log"] = "text/plain";
	_mimeTypes[".diff"] = "text/x-diff";
	_mimeTypes[".patch"] = "text/x-diff";
	_mimeTypes[".xhtml"] = "application/xhtml+xml";
	_mimeTypes[".php"] = "application/x-httpd-php";
	_mimeTypes[".plist"] = "application/xml";
	_mimeTypes[".sln"] = "text/plain";
	_mimeTypes[".tiff"] = "image/tiff";
	_mimeTypes[".app"] = "application/octet-stream";
	_mimeTypes[".ics"] = "text/calendar";
}
