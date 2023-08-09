#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <fstream>
# include <iostream>
# include <sstream>

# include <cstdio>
# include <cstdlib>
# include <cstring>

# include <algorithm>
# include <exception>
# include <list>
# include <map>
# include <string>
# include <vector>

# include <arpa/inet.h>
# include <fcntl.h>
# include <netinet/in.h>
# include <sys/poll.h>
# include <sys/socket.h>
# include <sys/stat.h>
# include <unistd.h>

// ===== ===== ===== ===== TYPEDEFS ===== ===== ===== =====
// forward declarations
class	Request;
class	Client;
class	Response;
class	Server;
class	ServerConfig;
class	ConfigFile;

typedef struct
{
	bool			get;
	bool			post;
	bool			delete_;
	std::string		dir_listing;
	std::string		alt_location;
}	s_locInfo;

typedef std::map<std::string, std::string> 				strMap;
typedef std::map<std::string, std::string>::iterator	strMap_it;
typedef std::vector<std::string>						strVec;
typedef std::vector<std::string>::iterator				strVec_it;
typedef std::map<std::string, s_locInfo>				strLocMap;
typedef std::map<std::string, s_locInfo>::iterator		strLocMap_it;
typedef	std::map<int, std::string>						intStrMap;
typedef	std::map<int, std::string>::iterator			intStrMap_it;
typedef std::vector<Client>								clientVec;
typedef std::vector<Client>::iterator					clientVec_it;

typedef enum contentTypes
{
	PLAINTEXT,
	HTML,
	CSS,
	OCTETSTREAM,
	ZIP,
	PNG,
	JPEG,
	PDF,
	XML,
	JSON,
	AVIF
}	contentType;


// Internal headers
# include "Request.hpp"
# include "Client.hpp"
# include "ServerConfig.hpp"
# include "ConfigFile.hpp"
# include "Response.hpp"
# include "Server.hpp"
# include "Url.hpp"

// MACROS
# define SERVERVERSION	"shmismserv v0.69"
# define WHITESPACE		" \t\v\r\n"
# define TERMINATION	"\r\n\r\n"
# define HTTPVERSION	"HTTP/1.1"
# define NOTFOUND		"_____TARGETSTRINGNOTFOUND_____"
# define ANNOUNCEME		std::cout << "Announcer: " << __FUNCTION__ << std::endl;

// ===== ===== ===== ===== SIZE Descriptions ===== ===== ===== =====
# define RECV_CHUNK_SIZE		4096
# define SEND_CHUNK_SIZE		4096
# define MAX_MAXCLIENTBODY		10000
# define MAX_MAXCONNECTIONS		1000
# define MAX_BACKLOG			100
# define MAX_HEADERSIZE			8192

// ===== ===== ===== ===== DEFAULT CONFIG PATHS ===== ===== ===== =====
# define PATH_DEFAULTCONFIG		"default/config/default.conf"
# define PATH_DEFAULTERRPAGE	"default/error/default.html"

// ===== ===== ===== ===== CONFIGURATION ELEMENT IDENTIFIERS ===== ===== ===== =====
# define SERVER		"server"
# define SERVERNAME	"server_name"
# define HOST		"host"
# define PORT		"listen"
# define ROOT		"root"
# define CLIMAXBODY	"client_max_body"
# define BACKLOG	"backlog"
# define MAXCONNS	"max_connections"

// ===== ===== ===== ===== CONFIGURATION SUB-ELEMENT IDENTIFIERS ===== ===== ===== =====
// error_page
# define ERRORPAGETITLE	"error_pages"
# define ERRPAGEDEFAULT	"default"
// location
# define LOCATIONTITLE	"location"
# define DIRLISTING		"dir_listing"
# define ALTLOC			"alt_location"
# define METHODS		"methods"
# define GET			"GET"
# define POST			"POST"
# define DELETE			"DELETE"
// cgi
# define CGITITLE		"cgi"
# define PYTHON			"py"
# define PHP			"php"

// ===== ===== ===== ===== ERROR MESSAGES ===== ===== ===== =====
// Global functions
# define E_INVALIDBRACE		"Error: webserv: getInstruction: Invalid use of curly brace: "
# define E_INVALIDENDTOKEN	"Error: webserv: getInstruction: Missing end token ('{', '}' or ';'): "

// ConfigFile
# define I_DEFAULTIMPORT	"Info: ConfigFile: Default ServerConfig successfully imported from "
# define I_CONFIGIMPORT		"Info: ConfigFile: " << _serverConfigs.size() << (_serverConfigs.size() == 1 ? " ServerConfig object" : " ServerConfig objects") <<" imported from " << userConfigPath << "."
# define E_ELMNTDECL		"Error: ConfigFile: Invalid element declaration, (only \"server\" allowed): "

// ServerConfig
# define E_FILEOPEN			"Error: ServerConfig: Could not open config file: "
# define E_NOSERVER			"Error: ServerConfig: No valid server configs found."
# define E_MANYSERVER		"Error: ServerConfig: Too many server configs found. Maximum of 10 allowed."
# define E_SUBELEMNT		"Error: ServerConfig: Second level subelements not allowed: "
# define E_INVALERRNUM		"Error: ServerConfig: Invalid HTML response code (range is from 100 to 599): "

# define I_INVALIDKEY		"Info: ServerConfig: Unrecognized identifier in config file: "
# define I_INVALIDVALUE		"Info: ServerConfig: Unrecognized value in config file: "
# define I_INVALERRPAGE		"Info: ServerConfig: Skipping invalid error page key-value pair."
# define I_INVALIDHEADER	"Info: ServerConfig: Elements between valid subelement headers and beginning of subelement (opening curly brace) will be ignored: "

// Server
# define E_SERVERNAME			"Error: Server: Invalid characters in server name input. Only alphanumerical, <<.>> and <<_>> allowed: "
# define E_HOSTADDRINPUT		"Error: Server: Invalid characters in host address input. Only numerical and dot allowed: "
# define E_HOSTADDRVAL			"Error: Server: Invalid address value. Cannot convert to IP address: "
# define E_PORTINPUT			"Error: Server: Invalid characters in port input. Only numerical allowed: "
# define E_PORTVAL				"Error: Server: Invalid port number. Must be from 0 to 65535: "
# define E_MAXCLIENTBODYINPUT	"Error: Server: Invalid characters in client max body size input. Only numerical allowed: "
# define E_MAXCLIENTBODYVAL		"Error: Server: Invalid client max body size: "
# define E_MAXCONNINPUT			"Error: Server: Invalid characters in max connections input. Only numerical allowed: "
# define E_MAXCONNVAL			"Error: Server: Invalid size of max connections: "
# define E_BACKLOGINPUT			"Error: Server: Invalid characters in backlog input. Only numerical allowed: "
# define E_BACKLOGVAL			"Error: Server: Invalid size of back log: "
# define E_ACC_READ				"Error: Server: Cannot read from path: "
# define E_ACC_WRITE			"Error: Server: Cannot write in path: "
# define E_ACC_EXEC				"Error: Server: Cannot execute from path: "
# define E_SOCKOPT				"Error: Server: setsockopt()"
# define E_FCNTL				"Error: Server: fcntl()"
# define E_ACCEPT				"Error: Server: accept()"
# define E_POLL					"Error: Server: poll()"
# define E_SEND					"Error: Server: send()"

# define I_CONNECTIONLIMIT		"Info: Server: Connection limit reached."
# define I_CLOSENODATA			"Info: Server: Connection closed (no data received)."
# define I_REQUESTHEADERROR		"Info: Server: Connection closed by request head handler: "

// Client
# define E_REQUESTFILE			"Error: Client: outputfile"

// ===== ===== ===== ===== GLOBAL FUNCTIONS ===== ===== ===== =====
// Determines whether a string contains purely alphanumerical characters.
bool 			isAlnumStr(const std::string &);
// Determines whether strings are the same (case-insensitive).
bool			isSameNoCase(std::string, std::string);
// Converts the alphabetical characters in a string to lowercase.
std::string		strToLower(std::string);
// Removes the characters defined by the macro WHITESPACE from the beginning and end of a string ref.
std::string		trim(std::string &);
// Returns a substring from the beginning of the passed string ref to the beginning of the first occurence of the 2nd argument. Deletes the substring and the 2nd argument from the passed string.
std::string		splitEraseStr(std::string &, const std::string&);
// Returns a substring from the beginning of the passed string ref to the first occurence of any char from the 2nd argument. Deletes the substring from the passed string, but not the encountered delimiting char. Calls trim function on both the remainder of the string ref and the returned element.
std::string		splitEraseTrimChars(std::string&, const std::string&);
// Returns a string vector. First argument is the string ref to operate on. 2nd argument is a string containing the characters of which any single one delimits the final strings. The 3rd argument denotes the end of the region to be processed.
strVec 			splitEraseStrVec(std::string& input, const std::string& targetChars, const std::string& endOfParsing);
// Determines the file/content type according to the file's full path. (based on dot-preceded extensions)
contentType		extensionType(const std::string &);
// Determines the size of a file.
off_t			fileSize(std::string);
// Splits a string according to a string, outputs vector of strings.
std::vector<std::string>	splitString(std::string, const std::string &);
// Splits a string ref and returns the first instruction it contains, defined as all characters from the string ref's start until the next top-level semicolon. Deletes the instruction from the string ref.
std::string		getInstruction(std::string& inputStr);
// Checks whether a resource exists, be it file or directory
bool			resourceExists(const std::string&);
// Checks whether a path corresponds to a directory
bool			isDirectory(const std::string&);
// Returns the size of a file. Returns (size_t)-1 on error.
size_t			getFileSize(const std::string&);
// Returns the message string associated with an HTTP status code
std::string		getHttpMsg(int);

#endif
