/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 20:51:05 by pandalaf          #+#    #+#             */
/*   Updated: 2023/08/05 22:37:28 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

#include <unistd.h>

Client::Client(int serverSocketfd, int pollStructIndex)
{
	_serverSocketfd = serverSocketfd;
	_pollStructIndex = pollStructIndex;
	_requestHeadComplete = false;
	_requestBodyComplete = false;
	_bodyBytesRead = 0;
}

void Client::setClientSocketfd(int clientSocketfd)
{
	_clientSocketfd = clientSocketfd;
}

void Client::handleRequestHead()
{
	ANNOUNCEME
	if (_requestHeadComplete)
		return;
	if (_buffer.find("\r\n\r\n") != std::string::npos)
	{
		_requestHead = RequestHead(_buffer);
		_buffer.erase(0, _buffer.find("\r\n\r\n") + 4);
		_requestHeadComplete = true;
		_bodyBytesRead += _buffer.size();
	}
}

void Client::appendToBuffer(std::string newData, int bytesReceived)
{
	_buffer.append(newData, 0, bytesReceived);
	if (_requestHeadComplete)
	{
		_bodyBytesRead += bytesReceived;
		_requestHeadComplete = _bodyBytesRead >= _requestHead.contentLength();
	}
}

bool Client::requestHeadComplete()
{
	return _requestHeadComplete;
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
	_requestHeadComplete = false;
	_bodyBytesRead = -1;
	//incomplete! but prolly obsolete
}
