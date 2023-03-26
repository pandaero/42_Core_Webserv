
#include "../include/RequestHandler.hpp"


RequestHandler::RequestHandler(void)
{}

RequestHandler::RequestHandler(std::string requestString):
	_requestString(requestString)
{}

RequestHandler::~RequestHandler(void)
{}

RequestHandler::RequestHandler(const RequestHandler& src)
{
	*this = src;
}

// Operator overloads
RequestHandler& RequestHandler::operator=(const RequestHandler& src)
{
	_requestString = src._requestString;
	_requestStruct = src._requestStruct;
	return *this;
}

/*
The first line of the client request is structured as follows:
SP = Space, CRLF = Carriage Return Line Feed (\r\n);
Method SP Request-URI SP HTTP-Version CRLF

*/

bool RequestHandler::parseMethod()
{
	std::string	element;
	std::string methodList[9]= {"GET", "POST", "PUT", "DELETE", "HEAD", "OPTIONS", "CONNECT", "TRACE", "PATCH"};
	size_t pos;
	int i = 0;
	
	element = _requestString.substr(0, pos = _requestString.find(' '));
	if (pos == std::string::npos)
	{
		std::cerr << "Empty client request. Aborting." << std::endl;
		return false;
	}
	while (i < 9 && element != methodList[i])
		i++;
	if (i == 9)
	{
		std::cerr << "Invalid method in client request. Defaulting to \"GET\"." << std::endl;
		_requestStruct.method = GET;
	}
	else
		_requestStruct.method = (reqMeth)i;
	return true;
}

bool RequestHandler::parseRequest()
{
	if (!parseMethod())
		return false;
	return true;
}

request RequestHandler::getRequest()
{
	parseRequest();
	return _requestStruct;
}


/*
GET /index.html HTTP/1.1
Host: www.example.com
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:68.0) Gecko/20100101 Firefox/68.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,;q=0.8
Accept-Language: en-US,en;q=0.5
Connection: keep-alive

*/