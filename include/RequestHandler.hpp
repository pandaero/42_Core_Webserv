
#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include <iomanip>
#include <ostream>
#include <iostream>
#include <fstream>
#include <exception>

typedef enum requestMethod
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

typedef struct request
{
	reqMeth		method;
	std::string	URI;
	std::string HTTPversion;
}	request;

class RequestHandler
{
	public:
		RequestHandler(void);
		RequestHandler(int);
		~RequestHandler(void);
		RequestHandler(const RequestHandler&);

		RequestHandler& operator=(const RequestHandler&);

		// Getters
		request getRequest();
	
	private:
		bool parseRequest(int);

		int _socket_fd;
		request _requestStruct;
};

#endif


/*
GET /index.html HTTP/1.1
Host: example.com
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:87.0) Gecko/20100101 Firefox/87.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate, br
Connection: keep-alive

*/