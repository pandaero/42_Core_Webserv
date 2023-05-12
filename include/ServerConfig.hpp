#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include <fstream>
# include <map>

# include "Server.hpp"
# include "webserv.hpp"

class ServerConfig
{
	public:
		ServerConfig();
		ServerConfig(std::string);
		ServerConfig(const ServerConfig&);
		~ServerConfig();

		ServerConfig& operator=(const ServerConfig&);
		
		void	applySettings(std::string);
		
		strMap		getConfigPairs() const;
		strMap		getErrorPaths() const;
		strLocMap	getLocations() const;
		strMap		getCgiPaths() const;

	private:
		void		parseDefaultErrorPages(std::string&);
		void 		parseUserErrorPages(std::string&);
		void		parseLocation(std::string&);
		void		parseDefaultCgi(std::string&);
		void		parseUserCgi(std::string&);
		std::string	getSubElement(std::string&);
		
		strMap		_configPairs;
		strMap		_errorPages;
		strLocMap	_locations;
		strMap		_cgiPaths;
};

#endif
