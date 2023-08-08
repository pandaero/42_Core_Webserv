#ifndef CONFIGFILE_HPP
# define CONFIGFILE_HPP

# include "webserv.hpp"

class ConfigFile
{
	public:
		ConfigFile(const char*);
		~ConfigFile();

		std::vector<ServerConfig>	getConfigs() const;
		std::vector<Server>			getServers() const;

	private:
		std::string					loadFile(const char*);
		std::string					getServerConfigElement(std::string&);
		
		ServerConfig*				_defaultServerConfig;
		std::vector<ServerConfig>	_serverConfigs;
		std::vector<Server>			_servers;
};

#endif
