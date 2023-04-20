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
#define E_FILEOPEN	"Could not open config file."
#define E_NOSERVER	"No valid server configs found."
#define E_BLOCTITLE	"Invalid block declaration, (only \"server\" allowed)."

class Server; //forward declaration necessary, but why?

struct serverConfig
{
	// Main Settings
	std::string	serverName;
	std::string	host;
	std::string port;
		
	// Bools
	bool get;
	bool post;
	bool delete_;
	bool dirListing;
	
	// Directories
	std::string root;
	std::string dir;
	std::string uploadDir;
	std::string cgiDir;
	std::string errorPage;
	
	// Size restrictions
	std::string clientMaxBody;
	std::string maxConnections;
	std::string backlog;

	serverConfig()
	{
		serverName = "unnamedServer";
		host = "0";
		port = "0";
		get = false;
		post = false;
		delete_ = false;
		dirListing = false;
		root = "website/root";
		dir = "website/dir";
		uploadDir = "website/upload";
		cgiDir = "website/cgi";
		errorPage = "website/error.html";
		clientMaxBody = "10000";
		maxConnections = "1000";
		backlog = "100";
	}
};

std::vector<Server> setupServers(const char*);

#endif
