#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include <fstream>
# include <map>

# include "Server.hpp"
# include "webserv.hpp"

class ServerConfig
{
	public:
		ServerConfig(std::string);
		~ServerConfig();

		strMap	getConfigPairs() const;
		strMap	getConfigErrorPaths() const;

	private:
		strMap	_configPairs;
		strMap	_errorPages;
		// locations configs
		// cgi configs
};

// Global functions for building ServerConfig objects
std::vector<ServerConfig>	getConfigs(const char*);
std::string					configFileToString(const char*);
std::string					getConfigElement(std::string&);

#endif
