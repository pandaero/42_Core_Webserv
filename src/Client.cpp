#include "../include/Client.hpp"

Client::Client(int pollStructIndex)
{
	fd = -42;
	_pollStructIndex = pollStructIndex;

	filePosition = 0;
	
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
	if (_request)
		delete _request;
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

std::string Client::directory()
{
	return _directory;
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
	_directory = _request->path().substr(0, _request->path().find_last_of("/") + 1);
	if (_request->contentLength() <= 0 || _request->method() != POST) // we don't process bodies of GET or DELETE requests
		requestBodyComplete = true;
	requestHeadComplete = true;
}

int Client::pollStructIndex()
{
	return _pollStructIndex;
}
