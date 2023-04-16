#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <fstream>

#include "Config.hpp"
#include "Server.hpp"
#include "Utils.hpp"

// Main settings
#define SERVERNAME	"serverName"
#define HOST		"host"
#define PORT		"port"

// Bools
#define GET			"GET"
#define POST		"POST"
#define DELETE		"DELETE"
#define DIRLISTING	"directoryListing"

// Directories
#define ROOT		"root"
#define DIR			"dir"
#define UPLOADDIR	"uploadDir"
#define CGIDIR		"cgiDir"
#define ERRORPAGE	"errorPage"

// Size limits
#define CLIMAXBODY	"clientMaxBodySize"

std::vector<Server> setupServers(const char*);

#endif
