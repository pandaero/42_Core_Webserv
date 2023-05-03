#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <fstream>

# include "Server.hpp"
# include "webserv.hpp"

class ServerConfig
{
	public:
		ServerConfig();
		ServerConfig(std::string);
		~ServerConfig();

		void setDefault();
		void setField(std::string, std::string);
		void parseConfigElement(std::string);

		std::string	serverName;
		std::string	host;
		std::string port;
		bool 		get;
		bool 		post;
		bool 		delete_;
		bool 		dirListing;
		std::string root;
		std::string dir;
		std::string uploadDir;
		std::string cgiDir;
		std::string	defaultErrorPage;
		std::map<size_t, std::string> errorPages;
		std::string clientMaxBody;
		std::string maxConnections;
		std::string backlog;
};

// Global functions for building ServerConfig objects
std::vector<ServerConfig>	getConfigs(const char*);
std::string					configFileToString(const char*);
std::string					getConfigElement(std::string&);

#endif
