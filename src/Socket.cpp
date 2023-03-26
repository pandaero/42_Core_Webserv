/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pandalaf <pandalaf@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 23:31:52 by pandalaf          #+#    #+#             */
/*   Updated: 2023/03/26 23:44:01 by pandalaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/webserv.hpp"
#include "../include/Socket.hpp"

#include <iostream>
#include <ctime>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void	startPolling(std::vector<Socket> & sockets)
{
	struct pollfd *	pollfds = new struct pollfd[sockets.size()];
	for (size_t i = 0; i < sockets.size(); ++i)
	{
		pollfds[i].fd = sockets[i].getSocketfd();
		pollfds[i].events = POLLIN;
		pollfds[i].revents = 0;
		if (listen(pollfds[i].fd, SOMAXCONN) == -1)
		{
			std::cerr << "Error: could not start listening on the socket " << pollfds[i].fd << "." << std::endl;
			perror("listen");
		}
	}
	while (true)
	{
		int polling = poll(pollfds, MAXPOLLFDS, -1);
		if (polling == -1)
			std::cerr << "Error: could not start polling on the sockets." << std::endl;
		else
		{
			for (size_t i = 0; i < sockets.size(); ++i)
			{
				if (pollfds[i].revents & POLLIN)
					sockets[i].getHandler()(pollfds[i].fd);
			}
		}
		if (g_polling == false)
		{
			delete [] pollfds;
			break;
		}
	}
	std::cout << "Polling..." << std::endl;
}

void	stopPolling()
{
	g_polling = false;
}

Socket::Socket():
	_socketfd(-1)
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

void	Socket::setHandler(void (&handler)(int))
{
	_handler = &handler;
}

int	Socket::getPort() const
{
	return (_port);
}

int	Socket::getSocketfd() const
{
	return (_socketfd);
}

void (*Socket::getHandler() const)(int)
{
	return (_handler);
}

void	Socket::bind(int port)
{
	struct sockaddr_in	server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_address.sin_port = htons(port);
	if (fcntl(_socketfd, F_SETFL, O_NONBLOCK) < 0)
		perror("fcntl");
	if (::bind(_socketfd, (struct sockaddr *) &server_address, sizeof(server_address)) == -1)
		throw bindingFailureException();
}

Socket::Socket(int socketfd):
	_handler(NULL),
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
