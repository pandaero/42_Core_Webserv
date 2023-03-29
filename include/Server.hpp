#ifndef SERVER_HPP
#define SERVER_HPP

#include <ostream>
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <map>
#include <vector>

typedef enum requestMethod //felt cute, might delete later
{
	GET,
	POST,
	PUT,
	DELETE,
	HEAD,
	OPTIONS,
	CONNECT,
	TRACE,
	PATCH
}	reqMeth;

typedef std::map<std::string, std::string>::const_iterator StringMap_it;
typedef std::map<std::string, std::string> StringMap;

typedef struct HTTPrequest
{
	std::string		Method;
	std::string		URI;
	std::string		HTTPversion;
	StringMap		Headers;
	std::string		Body;
}	HTTPrequest;

class Server
{
	public:
		Server(void);
		~Server(void);
		Server(const Server&);

		Server& operator=(const Server&);

		// Actions
		void parseRequest(std::string);
		void printRequest() const;

		// Getters
		HTTPrequest getRequest() const;

	
	private:
		// Utils
		std::string		splitEraseStr(std::string&, std::string);
		StringMap		splitEraseStrMap(std::string&, std::string, std::string, char);

		HTTPrequest		_request;
};

#endif
