/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 20:51:05 by pandalaf          #+#    #+#             */
/*   Updated: 2023/08/05 21:30:44 by wmardin          ###   ########.fr       */
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
	_bodyBytesRead = -1;
}

void Client::setClientSocketfd(int clientSocketfd)
{
	_clientSocketfd = clientSocketfd;
}

void Client::handleRequestHeader()
{
	if (_requestHeadComplete)
		return;
	if (_buffer.find("\r\n\r\n") != std::string::npos)
	{
		_requestHead = RequestHead(_buffer);
		_buffer.erase(0, _buffer.find("\r\n\r\n") + 4);
		_requestHeadComplete = true;
	}
}

void Client::appendToBuffer(std::string newData, int bytesReceived)
{
	_buffer.append(newData, bytesReceived);
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
