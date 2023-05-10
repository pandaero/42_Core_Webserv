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
		
		strMap	getConfigPairs() const;
		strMap	getConfigErrorPaths() const;

	private:
		void		parseErrorPages(std::string&);
		void		parseLocation(std::string&);
		void		parseCgi(std::string&);
		std::string	getSubElement(std::string&);
		
		strMap		_configPairs;
		strMap		_errorPages;
		strLocMap	_locations;
		// locations configs
		// cgi configs
};

#endif
