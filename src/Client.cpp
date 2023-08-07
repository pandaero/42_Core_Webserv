/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 20:51:05 by pandalaf          #+#    #+#             */
/*   Updated: 2023/08/07 11:09:36 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

#include <unistd.h>

Client::Client(int serverSocketfd, int pollStructIndex)
{
	_serverSocketfd = serverSocketfd;
	_clientSocketfd = -42;
	_pollStructIndex = pollStructIndex;
}

void Client::setSocketfd(int clientSocketfd)
{
	_clientSocketfd = clientSocketfd;
}

void Client::handleRequestHead()
{
	ANNOUNCEME
	if (_request.bodyComplete())
		return;
	if (_buffer.find("\r\n\r\n") != std::string::npos)
	{
		_request = Request(_buffer);
		if (requestBodyComplete())
			return;
		_buffer.erase(0, _buffer.find("\r\n\r\n") + 4);
	}
}

void Client::handleRequestBody()
{
	ANNOUNCEME
	if (_request.bodyComplete())
		return;
}


void Client::writeToBuffer(std::string newData, int bytesReceived)
{
	_buffer.append(newData, 0, bytesReceived);
}

// throw not yet caught
void Client::writeToFile()
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
	return _request.headComplete();
}

bool Client::requestBodyComplete()
{
	return _request.bodyComplete();
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
