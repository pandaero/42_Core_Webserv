/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 01:51:20 by pandalaf          #+#    #+#             */
/*   Updated: 2023/05/06 18:01:50 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

// Headers required by this header.
# include <iostream>
# include <vector>
# include <map>

// ===== ===== ===== ===== SIZE Descriptions ===== ===== ===== =====
# define RECV_CHUNK_SIZE		4096
# define SEND_CHUNK_SIZE		4096
# define MAX_MAXCLIENTBODY		10000
# define MAX_MAXCONNECTIONS		1000
# define MAX_BACKLOG			100

// ===== ===== ===== ===== DEFAULT CONFIG PATH ===== ===== ===== =====
# define DEFAULTCONFIGPATH		"default/config/default.conf"

// ===== ===== ===== ===== CONFIGURATION ELEMENT IDENTIFIERS ===== ===== ===== =====
# define SERVERNAME	"server_name"
# define HOST		"host"
# define PORT		"listen"
# define GET		"GET"
# define POST		"POST"
# define DELETE		"DELETE"
# define DIRLISTING	"dir_listing"
# define ROOT		"root"
# define DIR		"dir"
# define UPLOADDIR	"upload_dir"
# define CGIDIR		"cig_dir"
# define ERRORPAGE	"error_page"
# define CLIMAXBODY	"client_maxbody"
# define MAXCONNS	"max_connections"
# define BACKLOG	"backlog"
# define LOCATION	"location"
# define METHODS	"methods"
# define ALTLOC		"alt_location"

// ===== ===== ===== ===== ERROR MESSAGES ===== ===== ===== =====
// ServerConfig
# define E_FILEOPEN		"Error: ServerConfig: Could not open config file: "
# define E_NOSERVER		"Error: ServerConfig: No valid server configs found."
# define E_ELMNTDECL	"Error: ServerConfig: Invalid element declaration, (only \"server\" allowed): "
# define E_SUBELEMNT	"Error: ServerConfig: Subelements not allowed: "

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
typedef std::map<std::string, std::string>::const_iterator	StringMap_it;
typedef std::map<std::string, std::string> 					StringMap;

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
// Converts alphabetical characters to lowercase in a string.
std::string		strToLower(std::string);
// Description required.
std::string		trim(std::string &);
// Description required.
std::string		splitEraseStr(std::string &, std::string);
// Description required.
StringMap		createHeaderMap(std::string &, std::string, std::string, std::string);
// Determines the file/content type according to the file's full path. (based on dot-preceded extensions)
contentType		extensionType(const std::string &);
// Splits a string according to a string, outputs vector of strings.
std::vector<std::string>	splitString(std::string, const std::string &);
#endif
