#ifndef DEFINES_HPP
# define DEFINES_HPP

// general macros
# define SERVERVERSION	"webserv v0.69"
# define WHITESPACE		" \t\v\r\n"
# define HTTPVERSION	"HTTP/1.1"
# define ANNOUNCEME		//std::cout << "Announcer: " << __FUNCTION__ << std::endl;
# define ANNOUNCEME_FD	//std::cout << "Announcer: " << __FUNCTION__ << " on fd " << _clientIt->fd << std::endl;
# define SESSIONID		"sessionid"

// paths
# define SYS_CGIPAGE		"system/cgi.html"
# define SYS_TEMPCGIPOST	"system/temp/cgiPost_fd"
# define SYS_DIRLISTPAGE	"system/dirListing.html"
# define SYS_ERRPAGE		"system/errorPage.html"
# define SYS_DEFAULTCONF	"system/default.conf"
# define SYS_SCRIPT_PY		"system/cgi/python"
# define SYS_SCRIPT_PHP		"system/cgi/php"
# define SITE_LOGPAGE		"websites/site/sessionlog.html"

// numericals
# define RECV_CHUNK_SIZE		8192
# define SEND_CHUNK_SIZE		8192
# define MAX_MAXCLIENTBODY		4294967295 //2^32 - 1 -> max size_t @ 32 bit
# define MAX_MAXCONNECTIONS		1000
# define MAX_BACKLOG			100
# define MAX_HEADERSIZE			8192

// configuration element identifiers
# define SERVER				"server"
# define SERVERNAME			"server_name"
# define HOST				"host"
# define PORT				"listen"
# define ROOT				"root"
# define CLIMAXBODY			"client_max_body"
# define BACKLOG			"backlog"
# define MAXCONNS			"max_connections"
# define STDFILE			"standard_file"
# define UPLOADDIR			"upload_dir"
# define ERRORPAGETITLE		"error_pages"
# define ERRPAGEDEFAULT		"default"
# define LOCATIONTITLE		"location"
# define DIRLISTING			"dir_listing"
# define REDIRECTION		"http_redirect"
# define METHODS			"methods"
# define GET				"GET"
# define POST				"POST"
# define DELETE				"DELETE"
# define CGITITLE			"cgi"

// main
# define E_POLL				"Error: main: poll()"

// global functions
# define E_INVALIDBRACE		"Error: webserv: getInstruction: Invalid use of curly brace: "
# define E_INVALIDENDTOKEN	"Error: webserv: getInstruction: Missing end token ('{', '}' or ';'): "

// ConfigFile
# define I_CF_CONFIGIMPORT	"Info: ConfigFile: " << _servers.size() << " distinct"<< (_serverConfigs.size() == 1 ? " Server object" : " Server objects") <<" created using " << configPath << "."
# define E_CF_ELMNTDECL		"Error: ConfigFile: Invalid element declaration, (only \"server\" allowed): "
# define E_CF_NOSERVER		"Error: ConfigFile: No valid server configs found."
# define E_CF_MANYSERVER	"Error: ConfigFile: Too many server configs found. Maximum of 10 allowed."

// ServerConfig
# define E_SC_FILEOPEN		"Error: ServerConfig: Could not open config file: "
# define E_SC_SUBELEMNT		"Error: ServerConfig: Second level subelements not allowed: "
# define E_SC_INVALERRNUM	"Error: ServerConfig: Invalid HTML response code (range is from 100 to 599): "
# define E_SC_HOSTADDRVAL	"Error: ServerConfig: Invalid address value. Cannot convert to IP address: "
# define E_SC_PORTINPUT		"Error: ServerConfig: Invalid characters in port input. Only numerical allowed: "
# define E_SC_PORTVAL		"Error: ServerConfig: Invalid port number. Must be from 0 to 65535: "
# define E_SC_HOSTADDRVAL	"Error: ServerConfig: Invalid address value. Cannot convert to IP address: "
# define E_SC_ROOTINPUT		"Error: ServerConfig: Invalid characters in root input: "
# define E_SC_MAXCLBODINPUT	"Error: ServerConfig: Invalid characters in max client body input: "
# define E_SC_MAXCLBODHIGH	"Error: ServerConfig: Invalid client max body size (too high): "
# define E_SC_MAXCONNINPUT	"Error: ServerConfig: Invalid characters in max connections input. Only numerical allowed: "
# define E_SC_MAXCONNVAL	"Error: ServerConfig: Invalid size of max connections: "
# define E_SC_STDFILEINPUT	"Error: ServerConfig: Invalid characters in standard-file input: "

# define I_SC_INVALIDVALUE	"Info: ServerConfig: Unrecognized or invalid value in config file: "
# define I_SC_INVALIDKEY	"Info: ServerConfig: Unrecognized identifier in config file: "
# define I_SC_MISSINGVAL	"Info: ServerConfig: Missing value for key: "
# define I_SC_INVALSTATPAGE	"Info: ServerConfig: Skipping invalid status page key-value pair."
# define I_SC_INVALSERVNAME	"Info: ServerConfig: Skipping invalid server name: "
# define I_SC_NONDIRPATH	"Info: ServerConfig: Path does not end with a directory (no trailing '/'): "

// Server
# define E_ACCEPT				"Error: Server: accept()"
# define E_LISTEN				"Error: Server: listen()"
# define E_RECV					"Error: Server: recv()"
# define E_READ					"Error: Server: read()"
# define E_SEND					"Error: Server: send()"
# define E_WRITE_CHPIPE			"Error: Server: write() (to child pipe)"
# define E_TEMPFILE				"Error: Server: Could not create temporary file."
# define E_POSTFILE				"Error: Server: Could not create / open POST file."
# define E_PIPE					"Error: Server: pipe()."
# define E_FORK					"Error: Server: fork()."
# define E_EXECVE				"Error: Server: execve()."
# define E_DUP2					"Error: Server: dup2()."
# define E_FCNTL				"Error: Server: fcntl()."
# define E_CHILD				"Error: Server: child exited with error or was terminated."
# define E_IFSTREAM				"Error: Server: ifstream object"
# define E_CHILDTIMEOUT			"Error: Server: child timed out."

# define I_CONNECTIONLIMIT		"Info: Server: Connection limit reached."
# define I_CLOSENODATA			"Info: Server: Connection closed (no data received)."

#endif
