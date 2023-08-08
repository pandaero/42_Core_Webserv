/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 20:51:05 by pandalaf          #+#    #+#             */
/*   Updated: 2023/08/08 16:39:28 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

#include <unistd.h>

Client::Client(int pollStructIndex)
{
	_clientSocketfd = -42;
	_pollStructIndex = pollStructIndex;
	_bodyBytesWritten = 0;
	_requestHeadComplete = false;
	_requestBodyComplete = false;
	_request = NULL;
}

Client::~Client()
{
	if (_request)
		delete _request;
}

void Client::setSocketfd(int clientSocketfd)
{
	_clientSocketfd = clientSocketfd;
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
		
// change this to new alloc later
void Client::buildRequest()
{
	_request = new Request(_buffer);
	_buffer.erase(0, _buffer.find("\r\n\r\n") + 4);
	_requestHeadComplete = true;
	_directory = _request->path().substr(0, _request->path().find_last_of("/"));
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
	return _clientSocketfd;
}

const int& Client::pollStructIndex() const
{
	ANNOUNCEME
	return _pollStructIndex;
}
