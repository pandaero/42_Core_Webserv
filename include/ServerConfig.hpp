#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include "webserv.hpp"

class ServerConfig
{
	public:
		ServerConfig();
		ServerConfig(std::string, strMap*);
		ServerConfig(const ServerConfig&);

		ServerConfig& operator=(const ServerConfig&);
		
		void	applySettings(std::string);
		void	whoIsI();
		
		strMap		getConfigPairs() const;
		intStrMap	getErrorPaths() const;
		strLocMap	getLocations() const;
		strMap		getCgiPaths() const;
		strMap*		getMIMETypes() const;
		strVec		getNames() const;

		std::vector<ServerConfig>	getAltConfigs() const;
		void						addAltConfig(const ServerConfig&);

	private:
		void		parseDefaultErrorPages(std::string&);
		void 		parseUserErrorPages(std::string&);
		void		parseLocation(std::string&);
		void		parseDefaultCgi(std::string&);
		void		parseUserCgi(std::string&);
		void		parseNames(std::string&);
		
		strVec						_names;
		std::vector<ServerConfig>	_altConfigs;
		strMap						_configPairs;
		intStrMap					_errorPages;
		strLocMap					_locations;
		strMap						_cgiPaths;
		strMap*						_mimeTypes;
};

#endif
