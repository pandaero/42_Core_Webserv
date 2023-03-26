
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

// UTILS

std::string RequestHandler::splitspace()
{
	std::string element;
	size_t pos;

	element = _requestString.substr(0, pos = _requestString.find(' '));
	_requestString.erase(0, pos + 1);
	return element;
}

bool RequestHandler::parseMethod()
{
	std::string	method;
	std::string methodList[9]= {"GET", "POST", "PUT", "DELETE", "HEAD", "OPTIONS", "CONNECT", "TRACE", "PATCH"};
	size_t pos;
	size_t i = 0;
	
	method = splitspace();
	if (method.empty())
	{
		std::cerr << "Invalid client request. Aborting." << std::endl;
		return false;
	}
	while (i < 9 && method != methodList[i])
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

bool RequestHandler::parseURI()
{
	std::string URI;

	URI = splitspace();
	if (URI.empty())
	{
		std::cerr << "Invalid URI (for testing, prolly is not fatal?). Aborting." << std::endl;
		return false;
	}
	_requestStruct.URI = URI;
	return true;
}

bool RequestHandler::parseHTTP()
{
	std::string HTTP;

	HTTP = splitspace();
	if (HTTP.empty())
	{
		std::cerr << "Invalid HTTP Version (for testing, prolly is not fatal?). Aborting." << std::endl;
		return false;
	}
	_requestStruct.HTTPversion = HTTP;
	return true;
}

request RequestHandler::getRequest()
{
	if (!parseMethod())
		std::cout << "parseMethod failed." << std::endl;
	if (!parseURI())
		std::cout << "parseURI failed." << std::endl;
	if (!parseHTTP())
		std::cout << "parseHTTP failed." << std::endl;
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