/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pandalaf <pandalaf@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 17:49:49 by pandalaf          #+#    #+#             */
/*   Updated: 2023/04/07 19:20:38 by pandalaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

Server::Server(std::vector<std::string> serverParameters):
	_name(serverParameters[NAME]),
	_root(serverParameters[ROOT])
{
	if (serverParameters[ADDRESS] == "ANY")
		_serverAddress.sin_addr.s_addr = INADDR_ANY;
	else
	{
		in_addr_t	ipAddress = inet_addr(serverParameters[ADDRESS].c_str());
		if (ipAddress == -1)
			throw	invalidAddressException();
		_serverAddress.sin_addr.s_addr = ipAddress;
	}
	_serverAddress.sin_port = htons(std::atoi(serverParameters[PORT].c_str()));
	_pollStructs = new pollfd[std::atoi(serverParameters[MAXCONN])];
	_pollStructs[0].fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_pollStructs[0].fd == -1)
		throw	socketCreationFailureException();
	_pollStructs[0].events = POLLIN;
	_pollStructs[0].revents = 0;
	if (fcntl(_pollStructs[0].fd, F_SETFL, O_NONBLOCK) == -1)
		throw	fileDescriptorControlFailureException();
	if (bind(_pollStructs[0].fd, (struct sockaddr *) &_serverAddress, sizeof(_serverAddress)) == -1)
	{
		close(_pollStructs[0].fd);
		throw bindFailureException();
	}
	if (listen (_pollStructs[0].fd, SOMAXCONN) == -1)
	{
		close(_pollStructs[0].fd);
		throw listenFailureException();
	}
}

Server::~Server()
{
	delete [] _pollStructs;
}

void	Server::poll()
{
	if (::poll(_pollStructs, _numConns, -1) == -1)
		throw pollFailureException();
}

const char *	Server::invalidAddressException::what() const throw();
{
	return ("invalid IP address supplied.");
}

const char *	Server::socketCreationFailureException::what() const throw()
{
	return ("error creating socket for server.");
}

const char *	Server::fileDescriptorControlFailureException::what() const throw()
{
	return ("error controlling file descriptor to non-blocking.");
}

const char *	Server::bindFailureException::what() const throw()
{
	return ("error using bind.");
}

const char *	Server::listenFailureException::what() const throw()
{
	return ("error using listen.");
}

const char *	Server::pollFailureException::what() const throw()
{
	return ("error using poll.");
}
