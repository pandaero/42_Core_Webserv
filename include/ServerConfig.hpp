#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <fstream>

#include "Config.hpp"
#include "Server.hpp"
#include "Utils.hpp"

// Main settings
#define SERVERNAME	"server_name"
#define HOST		"host"
#define PORT		"listen"

// Bools
#define GET			"GET"
#define POST		"POST"
#define DELETE		"DELETE"
#define DIRLISTING	"directoryListing"

// Directories
#define ROOT		"root"
#define DIR			"dir"
#define UPLOADDIR	"uploadDir"
#define CGIDIR		"CGIdir"
#define ERRORPAGE	"errorPage"

// Size limits
#define CLIMAXBODY	"clientMaxBodySize"
#define MAXCONNS	"maxConnections"
#define BACKLOG		"backlog"

// Errors
#define E_FILEOPEN	"Could not open config file: "
#define E_NOSERVER	"No valid server configs found."
#define E_ELMNTDECL	"Invalid element declaration, (only \"server\" allowed): "
#define E_SUBELEMNT	"Subelements not allowed: "

class ServerConfig
{
	public:
		ServerConfig();
		ServerConfig(std::string);
		~ServerConfig();

		void setDefault();
		void setField(std::string, std::string);
		void parseInput(std::string);

		std::string	serverName;
		std::string	host;
		std::string port;
		bool get;
		bool post;
		bool delete_;
		bool dirListing;
		std::string root;
		std::string dir;
		std::string uploadDir;
		std::string cgiDir;
		std::string errorPage;
		std::string clientMaxBody;
		std::string maxConnections;
		std::string backlog;
};

// Global functions for building ServerConfig objects
std::vector<ServerConfig>	getConfigs(const char*);
std::string					configFileToString(const char*);
std::string					getConfigElement(std::string&);

#endif
