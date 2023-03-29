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

typedef struct HTTPrequest
{
	std::string							Method;
	std::string							URI;
	std::string							HTTPversion;
	std::map<std::string, std::string>	Headers;
	std::vector<std::string>			Body;
}	HTTPrequest;

typedef std::map<std::string, std::string>::const_iterator headers_it;

class Server
{
	public:
		Server(void);
		~Server(void);
		Server(const Server&);

		Server& operator=(const Server&);

		// Actions
		void parseRequest(std::string);
		void printRequest();

		// Getters
		HTTPrequest getRequest() const;

	
	private:
		// Utils
		std::string 						splitEraseStr(std::string&, std::string);
		std::map<std::string, std::string>	splitEraseStrMap(std::string&, std::string, std::string, char);

		HTTPrequest		_request;
};

#endif
