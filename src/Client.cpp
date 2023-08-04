/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 20:51:05 by pandalaf          #+#    #+#             */
/*   Updated: 2023/08/04 14:45:19 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

#include <unistd.h>

Client::Client(int serverSocketfd, int pollStructIndex)
{
	_clientSockLen = sizeof(_clientAddress); //not needed again, maybe not member var, just declare herE?
	_serverSocketfd = serverSocketfd;
	_pollStructIndex = pollStructIndex;
	_gotRequestHead = false;
	_bodyBytesRead = -1;
}

/* void Client::connect()
{
	ANNOUNCEME
	_clientSocketfd = accept(_serverSocketfd, (struct sockaddr *) &_clientAddress, &_clientSockLen);
	if (_clientSocketfd == -1)
		throw connectionDeniedException();
	std::cout << "New client accepted on fd " << _clientSocketfd << "." << std::endl;
} */

Client::~Client()
{
	std::cout << "Info: Client: Destructor called. " << std::endl;
}

sockaddr_in* Client::getSockaddr()
{
	return &_clientAddress;
}

int	Client::getSocketfd()
{
	return _clientSocketfd;
}

int Client::getPollStructIndex()
{
	return _pollStructIndex;
}

void Client::resetData()
{
	_buffer = "";
	_gotRequestHead = false;
	_bodyBytesRead = -1;
}

/* const char *	Client::connectionDeniedException::what() const throw()
{
	return ("Error: connection with the client was denied.");
}
 */