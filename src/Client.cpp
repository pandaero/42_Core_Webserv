/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 20:51:05 by pandalaf          #+#    #+#             */
/*   Updated: 2023/08/04 23:02:33 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

#include <unistd.h>

Client::Client(int serverSocketfd, int pollStructIndex)
{
	_serverSocketfd = serverSocketfd;
	_pollStructIndex = pollStructIndex;
	_gotRequestHead = false;
	_bodyBytesRead = -1;
}

Client::~Client()
{}

void Client::setClientSocketfd(int clientSocketfd)
{
	_clientSocketfd = clientSocketfd;
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
	return _pollStructIndex;
}

void Client::resetData()
{
	_buffer = "";
	_gotRequestHead = false;
	_bodyBytesRead = -1;
}
