#include "../include/webserv.hpp"
#include "../include/Socket.hpp"

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>


#include <sstream>
bool g_polling = true;

void	handleHttpRequest(int sockfd)
{
	char				buffer[1024];
	struct sockaddr_in	client_address;
	socklen_t			client_address_length = sizeof(client_address);
	int					bytesRcvd = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_address, &client_address_length);
	
	if (bytesRcvd == -1)
		std::cerr << "Error: did not receive data from socket." << std::endl;

	std::cout << "Received: " << buffer;

	std::string	received(buffer);
	
	std::string	response = "Received: \"" + received.substr(0, received.size() - 2) + "\" successfully.\n";
	send(sockfd, response.c_str(), response.size(), 0);
}

int	main(int argc, char **argv)
{

	(void) argc;
	(void) argv;

	Socket	http;
	http.bind(9445);
	http.setHandler(handleHttpRequest);

	Socket	whatevs;
	whatevs.bind(9334);
	whatevs.setHandler(handleHttpRequest);

	Socket	whatevselse;
	whatevselse.bind(9556);
	whatevselse.setHandler(handleHttpRequest);

	std::vector<Socket>	sockets;
	sockets.push_back(http);
	sockets.push_back(whatevs);
	sockets.push_back(whatevselse);

	startPolling(sockets);

	return (EXIT_SUCCESS);
}
