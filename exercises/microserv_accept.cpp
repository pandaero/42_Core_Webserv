#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

const int MAX_BUFFER_SIZE = 1024;
const int MAX_CONNECTIONS = 10;

int main(int argc, char ** argv)
{
	if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
		return (EXIT_FAILURE);
	}

	int port = std::atoi(argv[1]);

	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1)
	{
		std::cerr << "Failed to create socket: " << strerror(errno) << std::endl;
		return (EXIT_FAILURE);
	}

	sockaddr_in server_address;
	std::memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(port);

	int yes = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
	{
		std::cerr << "Failed to set socket option: " << strerror(errno) << std::endl;
		return (EXIT_FAILURE);
	}

	if (bind(server_fd, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address)) == -1)
	{
		std::cerr << "Failed to bind socket: " << strerror(errno) << std::endl;
		return 1;
	}

	if (listen(server_fd, MAX_CONNECTIONS) == -1)
	{
		std::cerr << "Failed to listen on socket: " << strerror(errno) << std::endl;
		return 1;
	}

	std::cout << "Server listening on port " << port << std::endl;

	while (true)
	{
		sockaddr_in	client_address;
		socklen_t	client_address_size = sizeof(client_address);
		int			client_fd = accept(server_fd, reinterpret_cast<sockaddr*>(&client_address), &client_address_size);
		if (client_fd == -1)
		{
			std::cerr << "Failed to accept connection: " << strerror(errno) << std::endl;
			continue;
		}

		std::cout << "Client connected: " << inet_ntoa(client_address.sin_addr) << ":" << ntohs(client_address.sin_port) << std::endl;

		char	buffer[MAX_BUFFER_SIZE];
		int		bytes_received = recv(client_fd, buffer, MAX_BUFFER_SIZE - 1, 0);
		if (bytes_received == -1)
		{
			std::cerr << "Failed to receive data: " << strerror(errno) << std::endl;
		}
		else
		{
			buffer[bytes_received] = '\0';
			std::cout << "Received " << bytes_received << " bytes: " << buffer << std::endl;
			if (send(client_fd, buffer, bytes_received, 0) == -1)
			{
				std::cerr << "Failed to send data: " << strerror(errno) << std::endl;
			}
		}
		close(client_fd);
	}
	close(server_fd);

	return (0);
}
