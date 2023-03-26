/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pandalaf <pandalaf@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 23:31:52 by pandalaf          #+#    #+#             */
/*   Updated: 2023/03/26 04:57:45 by pandalaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Socket.hpp"

#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>

std::vector<pthread_t> Socket::_socketThreads;

Socket::Socket():
	_socketfd(0)
{
	int	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketfd == -1)
		throw socketCreationFailureException();
	new (this) Socket(socketfd);
}

Socket::Socket(const Socket & other):
	_port(other._port),
	_socketfd(other._socketfd)
{

}

Socket::~Socket()
{
	close(_socketfd);
}

Socket &	Socket::operator=(const Socket & other)
{
	if (this != &other)
		new (this) Socket(other);
	return (*this);
}

void	Socket::bind(int port)
{
	struct sockaddr_in	server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(port);
	if (::bind(_socketfd, (struct sockaddr *) &server_address, sizeof(server_address)) == -1)
		throw bindingFailureException();
}

void	Socket::listen()
{
	if (::listen(_socketfd, SOCKETMAXCONNECTIONS) == -1)
		throw listenFailureException();
}

void	Socket::start(void * (&handler)(void *))
{
	struct s_client_info	client;
	client.length = sizeof(client.address);
	while(true)
	{
		client.fd = accept(_socketfd, (struct sockaddr *) &client.address, &client.length);
		if (client.fd == -1)
		{
			perror("accept");
			continue;
		}
		pthread_t	blankThread;
		_socketThreads.push_back(blankThread);
		pthread_t *	thread = &_socketThreads[_socketThreads.size() - 1];

		if (pthread_create(thread, NULL, handler, &client) != 0)
		{
			perror("pthread create");
			close (client.fd);
			continue;
		}
	}
}

void	Socket::stop()
{
	if (shutdown(_socketfd, SHUT_RDWR) == -1)
		std::cerr << "Error: socket: could not shut down properly." << std::endl;
	for (size_t i = 0; i < _socketThreads.size(); ++i)
	{
		int ret = pthread_join(_socketThreads[i], NULL);
		switch (ret)
		{
			// case EDEADLK:
			// 	std::cerr << "Error: socket: thread deadlock." << std::endl;
			// 	break;
			// case EINVAL:
			// 	std::cerr << "Error: socket: thread joining prevented." << std::endl;
			// 	break;
			// case ESRCH:
			// 	std::cerr << "Error: socket: thread not found." << std::endl;
			// 	break;
			default:
				std::cerr << "Error: socket: thread malfunction." << std::endl;
				break;
		}
	}
}

Socket::Socket(int socketfd):
	_socketfd(socketfd)
{

}

const char *	Socket::socketCreationFailureException::what() const throw()
{
	return("Error: socket: failed to create socket.");
}

const char *	Socket::bindingFailureException::what() const throw()
{
	return("Error: socket: failed to bind socket to port.");
}

const char *	Socket::listenFailureException::what() const throw()
{
	return("Error: socket: failed to set socket to listen for incoming connections.");
}
