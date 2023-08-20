#ifndef DEFINES_HPP
# define DEFINES_HPP

// Macros
# define SERVERVERSION	"webserv v0.69"
# define WHITESPACE		" \t\v\r\n"
# define HTTPVERSION	"HTTP/1.1"
# define NOTFOUND		"_____TARGETSTRINGNOTFOUND_____"
# define ANNOUNCEME		std::cout << "Announcer: " << __FUNCTION__ << std::endl;
# define ANNOUNCEME_FD	std::cout << "Announcer: " << __FUNCTION__ << " on fd " << _clientIt->fd << std::endl;

// ===== ===== ===== ===== Directories ===== ===== ===== =====
# define SYS_ROOT	"./system"

// ===== ===== ===== ===== SIZE Descriptions ===== ===== ===== =====
# define RECV_CHUNK_SIZE		4096
# define SEND_CHUNK_SIZE		4096
# define MAX_MAXCLIENTBODY		1e6
# define MAX_MAXCONNECTIONS		1000
# define MAX_BACKLOG			100
# define MAX_HEADERSIZE			8192

// ===== ===== ===== ===== CONFIGURATION ELEMENT IDENTIFIERS ===== ===== ===== =====
# define SERVER		"server"
# define SERVERNAME	"server_name"
# define HOST		"host"
# define PORT		"listen"
# define ROOT		"root"
# define CLIMAXBODY	"client_max_body"
# define BACKLOG	"backlog"
# define MAXCONNS	"max_connections"
# define STDFILE	"standard_file"
# define UPLOADDIR	"upload_dir"

// ===== ===== ===== ===== CONFIGURATION SUB-ELEMENT IDENTIFIERS ===== ===== ===== =====
// error_page
# define ERRORPAGETITLE	"error_pages"
# define ERRPAGEDEFAULT	"default"
// location
# define LOCATIONTITLE	"location"
# define DIRLISTING		"dir_listing"
# define REDIRECTION	"http_redirect"
# define METHODS		"methods"
# define GET			"GET"
# define POST			"POST"
# define DELETE			"DELETE"
// cgi
# define CGITITLE		"cgi"
# define PYTHON			"py"
# define PHP			"php"

// ===== ===== ===== ===== ERROR MESSAGES ===== ===== ===== =====
// Main
# define E_POLL				"Error: main: poll()"

// Global functions
# define E_INVALIDBRACE		"Error: webserv: getInstruction: Invalid use of curly brace: "
# define E_INVALIDENDTOKEN	"Error: webserv: getInstruction: Missing end token ('{', '}' or ';'): "

// ConfigFile
# define I_DEFAULTIMPORT	"Info: ConfigFile: Default ServerConfig successfully imported from "
# define I_CONFIGIMPORT		"Info: ConfigFile: " << _servers.size() << " distinct"<< (_serverConfigs.size() == 1 ? " Server object" : " Server objects") <<" created using " << userConfigPath << "."
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
# define I_INVALSERVERNAME	"Info: ServerConfig: Skipping invalid server name: "

// Server
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
# define E_SOCKET				"Error: Server: socket()"
# define E_BIND					"Error: Server: bind()"
# define E_FCNTL				"Error: Server: fcntl()"
# define E_ACCEPT				"Error: Server: accept()"
# define E_LISTEN				"Error: Server: listen()"
# define E_SEND					"Error: Server: send()"
# define E_TEMPFILE				"Error: Server: Could not create error page file."

# define I_CONNECTIONLIMIT		"Info: Server: Connection limit reached."
# define I_CLOSENODATA			"Info: Server: Connection closed (no data received)."
# define I_REQUESTHEADERROR		"Info: Server: Connection closed by request head handler: "

// Client
# define E_REQUESTFILE			"Error: Client: outputfile"

#endif
