#include "../include/Client.hpp"

Client::Client(int index)
{
	(void)index;
	fd = -42;

	filePosition = 0;
	bytesWritten = 0;
	
	_request = NULL;

	errorPending = false;
	requestHeadComplete = false;
	requestBodyComplete = false;
	responseFileSelected = false;
	responseHeadSent = false;
	responseBodySent = false;

	statusCode = 0;
}

Client::~Client()
{
	std::cout << "client destructor fd " << fd << std::endl;
/* 	if (_request)
	{
		delete _request;
		_request = NULL;
	} */
}

std::string Client::httpProtocol()
{
	return _request->httpProtocol();
}

std::string Client::method()
{
	return _request->method();
}

std::string Client::path()
{
	return _request->path();
}

int Client::contentLength()
{
	return _request->contentLength();
}

std::string Client::contentType()
{
	return _request->contentType();
}
		
void Client::buildRequest()
{
	_request = new Request(buffer);
	buffer.erase(0, buffer.find("\r\n\r\n") + 4);
	if (_request->path().find("/") == std::string::npos)
		throw std::runtime_error("invalid URL in request.");
	directory = _request->path().substr(0, _request->path().find_last_of("/") + 1);
	filename = _request->path().substr(_request->path().find_last_of("/"));
	if (_request->contentLength() <= 0 || _request->method() != POST) // we don't process bodies of GET or DELETE requests
		requestBodyComplete = true;
	requestHeadComplete = true;
}

