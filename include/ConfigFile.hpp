#ifndef CONFIGFILE_HPP
# define CONFIGFILE_HPP

# include "webserv.hpp"

class ConfigFile
{
	public:
		ConfigFile(const char*);

		std::vector<ServerConfig>	getConfigs() const;
		std::vector<Server>&		getServers();

	private:
		std::string					loadFile(const char*);
		void 						setMIMEtypes();
		std::string					getServerConfigElement(std::string&);
		bool						combineSharedNetAddr(const ServerConfig&, size_t);
		bool						sharedNetAddr(const ServerConfig&, const ServerConfig&);
		
		std::vector<ServerConfig>	_serverConfigs;
		std::vector<Server>			_servers;
		strMap						_mimeTypes;
};

#endif
