/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 01:51:20 by pandalaf          #+#    #+#             */
/*   Updated: 2023/05/12 20:06:33 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

// Headers required by this header.
# include <iostream>
# include <vector>
# include <map>

// PSEUDOVARIABLES
# define WHITESPACE		" \t\v\r\n"
# define TERMINATION	"\r\n\r\n"

// ===== ===== ===== ===== SIZE Descriptions ===== ===== ===== =====
# define RECV_CHUNK_SIZE		4096
# define SEND_CHUNK_SIZE		4096
# define MAX_MAXCLIENTBODY		10000
# define MAX_MAXCONNECTIONS		1000
# define MAX_BACKLOG			100

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
// ConfigFile
# define E_INVALBRACES		"Error: ConfigFile: Invalid placement of curly braces: "

# define I_DEFAULTIMPORT	"Info: ConfigFile: Default ServerConfig successfully imported from "
# define I_CONFIGIMPORT		"Info: ConfigFile: " << _serverConfigs.size() << (_serverConfigs.size() == 1 ? " config element" : " config elements") <<" imported from " << path << "."

// ServerConfig
# define E_FILEOPEN		"Error: ServerConfig: Could not open config file: "
# define E_NOSERVER		"Error: ServerConfig: No valid server configs found."
# define E_ELMNTDECL	"Error: ServerConfig: Invalid element declaration, (only \"server\" allowed): "
# define E_SUBELEMNT	"Error: ServerConfig: Second level subelements not allowed: "
# define E_INVALERRNUM	"Error: ServerConfig: Invalid HTML response code (range is from 100 to 599): "

# define I_INVALIDKEY	"Info: ServerConfig: Unrecognized identifier in config file: "

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

// ===== ===== ===== ===== TYPEDEFS ===== ===== ===== =====
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

// ===== ===== ===== ===== GLOBAL FUNCTIONS ===== ===== ===== =====
// Determines whether a string contains purely alphanumerical characters.
bool 			isAlnumStr(const std::string &);
// Determines whether strings are the same (case-insensitive).
bool			isSameNoCase(std::string, std::string);
// Converts the alphabetical characters in a string to lowercase.
std::string		strToLower(std::string);
// Removes the characters defined by the macro WHITESPACE from the beginning and end of a string.
std::string		trim(std::string &);
// Returns a substring from the beginning of the passed string to the beginning of the first occurence of the 2nd argument. Deletes the substring and the 2nd argument from the passed string.
std::string		splitEraseStr(std::string &, std::string);
// Returns a substring from the beginning of the passed string to the first occurence of any char from the 2nd argument. Deletes the substring and the encountered char from the passed string.
std::string		splitEraseChars(std::string&, std::string);
// Returns a string-string map. First argument is the input. 2nd and 3rd arguments are the end of the key and the end of the value. 4th argument signals the end of the region to be parsed. The key is converted to lower case.
strMap		createHeaderMap(std::string &, std::string, std::string, std::string);
// Determines the file/content type according to the file's full path. (based on dot-preceded extensions)
contentType		extensionType(const std::string &);
// Splits a string according to a string, outputs vector of strings.
std::vector<std::string>	splitString(std::string, const std::string &);

#endif
