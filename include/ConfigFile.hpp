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
		void 						setMIMEtypes();
		std::string					getServerConfigElement(std::string&);
		bool						combineSharedNetAddr(const ServerConfig&, size_t);
		bool						sharedNetAddr(const ServerConfig&, const ServerConfig&);
		
		ServerConfig*				_defaultServerConfig;
		std::vector<ServerConfig>	_serverConfigs;
		std::vector<Server>			_servers;
		strMap						_mimeTypes;
};

#endif
