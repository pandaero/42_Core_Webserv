
#include "../include/RequestHandler.hpp"

RequestHandler::RequestHandler(void)
{
	_socket_fd = -1;
}

RequestHandler::RequestHandler(int socket_fd):
	_socket_fd(socket_fd)
{

}

RequestHandler::~RequestHandler(void)
{}

RequestHandler::RequestHandler(const RequestHandler& src)
{
	*this = src;
}

// Operator overloads
RequestHandler& RequestHandler::operator=(const RequestHandler& src)
{
	_requestStruct = src._requestStruct;
	return *this;
}

/*
The first line of the client request is structured as follows:
SP = Space, CRLF = Carriage Return Line Feed (\r\n);
Method SP Request-URI SP HTTP-Version CRLF

*/

bool RequestHandler::parseRequest()
{
/* 	std::ifstream clientRequest("input.txt");
	std::string line;
*/
	FILE* clientRequest = fdopen(_socket_fd, "r");
	std::ifstream infile(*clientRequest);
	std::string	line;
	
	while (std::getline(clientRequest, line))
	{
		size_t delim = line.find(',');
		if (delim == std::string::npos)
			continue;
	}
}


/*
GET /index.html HTTP/1.1
Host: www.example.com
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:68.0) Gecko/20100101 Firefox/68.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
Accept-Language: en-US,en;q=0.5
Connection: keep-alive

*/