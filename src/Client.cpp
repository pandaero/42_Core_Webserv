#include "../include/Client.hpp"

Client::Client(int pollStructIndex)
{
	_socketfd = -42;
	_pollStructIndex = pollStructIndex;
	_bodyBytesHandled = 0;

	state = requestHead;

	_receivingComplete = false;
	_requestHeadComplete = false;
	_requestBodyComplete = false;
	_responseFileSelected = false;
	_sendingComplete = false;
	_request = NULL;
}

Client::~Client()
{
	if (_request)
		delete _request;
}

void Client::setSocketfd(int clientSocketfd)
{
	_socketfd = clientSocketfd;
}

void Client::setReceivingComplete()
{
	_requestBodyComplete = true;
	_receivingComplete = true;
}

std::string& Client::buffer()
{
	return _buffer;
}

bool Client::receivingComplete()
{
	return _receivingComplete;
}

bool Client::responseFileSelected()
{
	return _responseFileSelected;
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
		_requestBodyComplete = true;
	_requestHeadComplete = true;
}

bool Client::requestHeadComplete()
{
	return _requestHeadComplete;
}

bool Client::requestBodyComplete()
{
	return _requestBodyComplete;
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
