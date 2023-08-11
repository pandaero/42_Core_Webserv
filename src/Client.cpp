#include "../include/Client.hpp"

int Client::clientCount = 0;

Client::Client(int pollStructIndex)
{
	clientNumber = clientCount++;
	
	_socketfd = -42;
	_pollStructIndex = pollStructIndex;
	_bodyBytesHandled = 0;

	//state = receiveRequestHead;
	filePosition = 0;
	
	_request = NULL;

	requestHeadComplete = false;
	requestBodyComplete = false;
	responseFileSelected = false;
	responseHeadSent = false;
}

Client::~Client()
{
	/* if (_request)
		delete _request; */
}

void Client::setSocketfd(int clientSocketfd)
{
	_socketfd = clientSocketfd;
}

std::string& Client::buffer()
{
	return _buffer;
}

const std::string& Client::httpProtocol() const
{
	return _request->httpProtocol();
}

const std::string& Client::method() const
{
	return _request->method();
}

const std::string& Client::path() const
{
	return _request->path();
}

const std::string& Client::directory() const
{
	return _directory;
}

const int& Client::contentLength() const
{
	return _request->contentLength();
}

const std::string& Client::contentType() const
{
	return _request->contentType();
}
		
void Client::buildRequest()
{
	_request = new Request(_buffer);
	_buffer.erase(0, _buffer.find("\r\n\r\n") + 4);
	_directory = _request->path().substr(0, _request->path().find_last_of("/") + 1);
	if (_request->contentLength() <= 0 || _request->method() != POST) // we don't process bodies of GET or DELETE requests
		requestBodyComplete = true;
	requestHeadComplete = true;
}

sockaddr_in* Client::sockaddr()
{
	return &_clientAddress;
}

const int&	Client::socketfd() const
{
	return _socketfd;
}

const int& Client::pollStructIndex() const
{
	return _pollStructIndex;
}
