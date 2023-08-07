/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 20:51:05 by pandalaf          #+#    #+#             */
/*   Updated: 2023/08/06 21:51:07 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

#include <unistd.h>

Client::Client(int serverSocketfd, int pollStructIndex)
{
	_serverSocketfd = serverSocketfd;
	_pollStructIndex = pollStructIndex;
	_RequestComplete = false;
	_requestBodyComplete = false;
	_bodyBytesRead = 0;
}

void Client::setClientSocketfd(int clientSocketfd)
{
	_clientSocketfd = clientSocketfd;
}

void Client::handleRequest()
{
	ANNOUNCEME
	if (_RequestComplete)
		return;
	if (_buffer.find("\r\n\r\n") != std::string::npos)
	{
		_Request = Request(_buffer);
		_buffer.erase(0, _buffer.find("\r\n\r\n") + 4);
		_RequestComplete = true;
		_bodyBytesRead += _buffer.size();
	}
}

void Client::appendToBuffer(std::string newData, int bytesReceived)
{
	_buffer.append(newData, 0, bytesReceived);
	if (_RequestComplete)
		_bodyBytesRead += bytesReceived;
	// but not useful for chunking there has to be another var in the headers for chunking.
	// i think this is necessary because in case of super big headers, the body 
	// may not be read completely in one go even tho it is not in itself too big 
	_requestBodyComplete = _bodyBytesRead >= _Request.contentLength();
}

bool Client::RequestComplete()
{
	return _RequestComplete;
}

bool Client::requestBodyComplete()
{
	return _requestBodyComplete;
}

sockaddr_in* Client::getSockaddr()
{
	return &_clientAddress;
}

int	Client::getSocketfd() const
{
	return _clientSocketfd;
}

int Client::getPollStructIndex() const
{
	ANNOUNCEME
	return _pollStructIndex;
}

void Client::resetData()
{
	_buffer = "";
	_RequestComplete = false;
	_bodyBytesRead = -1;
	//incomplete! but prolly obsolete
}
