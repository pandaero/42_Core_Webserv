#include "../include/webserv.hpp"
#include "../include/Socket.hpp"

#include <iostream>
#include <cstdlib>
#include <unistd.h>

void *	handleHttpRequest(void *arg)
{
	struct s_client_info * client;
	client = (struct s_client_info *) arg;

	char	buffer[100];
	read(client->fd, buffer, 100);
	std::cout << "Received: " << buffer;
	std::string	received(buffer);
	std::string	response = "Received: \"" + received.substr(0, received.size() - 2) + "\" successfully.\n";
	send(client->fd, response.c_str(), response.size(), 0);
	close(client->fd);
	return (NULL);
}

int	main(int argc, char **argv)
{
	(void) argc;
	(void) argv;

	Socket	http;
	http.bind(9999);
	http.listen();
	http.start(handleHttpRequest);

	return (EXIT_SUCCESS);
}
