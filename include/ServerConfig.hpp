#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include "webserv.hpp"

class ServerConfig
{
	public:
		ServerConfig();
		ServerConfig(std::string);
		ServerConfig(const ServerConfig&);

		ServerConfig& operator=(const ServerConfig&);
		
		void	applySettings(std::string);
		void	whoIsI();
		
		strMap		getConfigPairs() const;
		intStrMap	getErrorPaths() const;
		strLocMap	getLocations() const;
		strMap		getCgiPaths() const;

	private:
		void		parseDefaultErrorPages(std::string&);
		void 		parseUserErrorPages(std::string&);
		void		parseLocation(std::string&);
		void		parseDefaultCgi(std::string&);
		void		parseUserCgi(std::string&);
		
		strMap		_configPairs;
		intStrMap	_errorPages;
		strLocMap	_locations;
		strMap		_cgiPaths;
};

#endif
