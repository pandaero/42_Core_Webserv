/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 20:51:05 by pandalaf          #+#    #+#             */
/*   Updated: 2023/06/13 22:01:36 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

#include <unistd.h>

Client::Client(int serverSocketfd)
{
	_clientSocketfd = accept(serverSocketfd, (struct sockaddr *) &_clientAddress, &_clientSockLen);
	if (_clientSocketfd == -1)
	{
		close(_clientSocketfd);
		throw connectionDeniedException();
	}
	_gotRequestHead = false;
	_bodyBytesRead = -1;
}

Client::~Client()
{
	close(_clientSocketfd);
	std::cout << "Info: Client: Destructor: Closed Client Socket FD " << _clientSocketfd << "." << std::endl;
}

int	Client::getSocketfd()
{
	return (_clientSocketfd);
}

void Client::resetData()
{
	_buffer = "";
	_gotRequestHead = false;
	_bodyBytesRead = -1;
}

const char *	Client::connectionDeniedException::what() const throw()
{
	return ("Error: connection with the client was denied.");
}
