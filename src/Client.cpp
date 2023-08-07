/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 20:51:05 by pandalaf          #+#    #+#             */
/*   Updated: 2023/08/07 19:48:41 by wmardin          ###   ########.fr       */
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
}

void Client::setSocketfd(int clientSocketfd)
{
	_clientSocketfd = clientSocketfd;
}

void Client::handleRequestHead()
{
	ANNOUNCEME
	if (requestHeadComplete())
		return;
	if (_buffer.find("\r\n\r\n") != std::string::npos)
	{
		_request = Request(_buffer);
		_buffer.erase(0, _buffer.find("\r\n\r\n") + 4);
		_requestHeadComplete = true;
	}
}

std::string& Client::buffer()
{
	return _buffer;
}

void Client::handleRequestBody()
{
	ANNOUNCEME
	if (requestBodyComplete())
		return;
}

// throw not yet caught
void Client::writeBodyToFile()
{
	//now just taking request path, have to modify this with the config file given directory
	std::string		writePath(_request.path());

	std::ofstream	outputFile(writePath.c_str(), std::ios::binary | std::ios::app);
	
	if (!outputFile.is_open())
		throw (E_REQUESTFILE);
	outputFile.write(_buffer.c_str(), _buffer.size());
	outputFile.close();
	_request.addToBodyBytesWritten(_buffer.size()); // outputFile.tellp() delta prolly better but nah.
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

int	Client::socketfd() const
{
	return _clientSocketfd;
}

int Client::pollStructIndex() const
{
	ANNOUNCEME
	return _pollStructIndex;
}
