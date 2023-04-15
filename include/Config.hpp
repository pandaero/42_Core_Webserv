#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <fstream>

#include "Config.hpp"
#include "Server.hpp"
#include "Utils.hpp"

#define SERVERNAME	"serverName"
#define HOST		"host"
#define PORT		"port"
#define CLIMAXBODY	"clientMaxBodySize"
#define GET			"GET"
#define POST		"POST"
#define DELETE		"DELETE"
#define ERRORPAGE	"errorPage"
#define DIRLISTING	"directoryListing"
#define ROOT		"root"
#define DIR			"dir"
#define UPLOADDIR	"uploadDir"

struct serverConfig //not needed, just directy populate the server
{
	std::string		_name;
	in_addr_t		_host;
	uint16_t		_port;
	std::string		_errorPage;
	size_t			_maxClientBodySize;
	std::string		_directoryListing;
	std::string		_root;
	std::string		_dir;
	std::string		_uploadDir;	
};

void parseConfigFile(const char*);

#endif

/*
chat config for apache:
# Set the server name
ServerName example.com

# Set the document root directory
DocumentRoot /var/www/html

# Set the default file to serve
DirectoryIndex index.html

# Configure access control
<Directory /var/www/html>
    AllowOverride None
    Require all granted
</Directory>

# Enable server-side scripting
AddHandler cgi-script .cgi

# Enable SSL/TLS encryption
SSLEngine on
SSLProtocol all -SSLv3 -TLSv1 -TLSv1.1
SSLHonorCipherOrder on
SSLCipherSuite ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES256-GCM-SHA384:ECDHE-RSA-AES256-GCM-SHA384:DHE-RSA-AES128-GCM-SHA256:DHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-AES128-SHA256:ECDHE-RSA-AES128-SHA256:ECDHE-ECDSA-AES256-SHA384:ECDHE-RSA-AES256-SHA384:DHE-RSA-AES128-SHA256:DHE-RSA-AES256-SHA256

# Redirect non-HTTPS traffic to HTTPS
RewriteEngine on
RewriteCond %{HTTPS} off
RewriteRule (.*) https://%{HTTP_HOST}%{REQUEST_URI} [R=301,L]

*/
