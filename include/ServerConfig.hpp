#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include "webserv.hpp"

class ServerConfig
{
	public:
		ServerConfig(std::string, strMap*);

		strVec		getNames() const;
		in_addr_t	getHost() const;
		in_port_t	getPort() const;
		std::string	getRoot() const;
		bool		getDefaultDirlisting() const;
		uint64_t	getClientMaxBody() const;
		size_t		getMaxConnections() const;
		std::string	getStandardFile() const;
		intStrMap	getStatusPagePaths() const;
		strLocMap	getLocations() const;
		strMap		getCgiPaths() const;
		strMap*		getMIMETypes() const;
		servConfVec	getAltConfigs() const;

		void		addAltConfig(const ServerConfig&);

	private:
		void parseNames(std::string&);
		void parseHost(std::string&);
		void parsePort(std::string&);
		void parseRoot(std::string&);
		void parseDirListing(std::string&);
		void parseClientMaxBody(std::string&);
		void parseMaxConnections(std::string&);
		void parseStandardFile(std::string&);
		void parseStatusPagePaths(std::string&);
		void parseLocations(std::string&);
		void parseCgiPaths(std::string&);
		
		strVec			_names;
		in_addr_t		_host;
		in_port_t		_port;
		std::string		_root;
		bool			_defaultDirListing;
		uint64_t		_clientMaxBody;
		size_t			_maxConnections;
		std::string		_standardFile;
		intStrMap		_statusPagePaths;
		strLocMap		_locations;
		strMap			_cgiPaths;
		servConfVec		_altConfigs;
		strMap*			_mimeTypes;
};

#endif
