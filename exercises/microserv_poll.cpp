#include <iostream>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>

#define MAXPOLLFDS 10

int main(int argc, char ** argv)
{
	if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
		return (EXIT_FAILURE);
	}

    std::vector<int> clients;
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    
    // create server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        std::cerr << "Error: could not create server socket." << std::endl;
        return (EXIT_FAILURE);
    }
    
    // bind socket to port
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(std::atoi(argv[1]));
    if (bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address)) == -1)
    {
        std::cerr << "Error: could not bind server socket to port." << std::endl;
        close(server_socket);
        return (EXIT_FAILURE);
    }
    
    // start listening for incoming connections
    if (listen(server_socket, 5) == -1)
    {
        std::cerr << "Error: could not start listening on server socket." << std::endl;
        close(server_socket);
        return (EXIT_FAILURE);
    }

    // initialize pollfd structs
    struct pollfd pollfds[MAXPOLLFDS];
    memset(pollfds, 0, sizeof(pollfds));
    pollfds[0].fd = server_socket;
    pollfds[0].events = POLLIN;
    
    int nfds = 1;
    
    // wait for incoming connections using poll
    std::cout << "Server is listening for incoming connections..." << std::endl;
    while (true)
    {
        int polling = poll(pollfds, nfds, -1);
        if (polling == -1)
        {
            std::cerr << "Error: could not start polling on the server socket." << std::endl;
            close(server_socket);
            return (EXIT_FAILURE);
        }

        for (int i = 0; i < nfds; ++i)
        {
            if (pollfds[i].revents & POLLIN)
            {
                // check if new client is connecting
                if (pollfds[i].fd == server_socket)
                {
                    socklen_t client_address_len = sizeof(client_address);
                    client_socket = accept(server_socket, (struct sockaddr*) &client_address, &client_address_len);
                    if (client_socket == -1)
                    {
                        std::cerr << "Error: could not accept incoming connection." << std::endl;
                    }
                    else
                    {
                        std::cout << "New client connected." << std::endl;
                        clients.push_back(client_socket);
                        pollfds[nfds].fd = client_socket;
                        pollfds[nfds].events = POLLIN;
                        ++nfds;
                    }
                }
                else // data is ready to be read from a client socket
                {
                    int client_index = -1;
                    for (int j = 0; j < clients.size(); ++j)
					{
						if (clients[j] != -1 && pollfds[j].fd)
						{
							client_index = j;
							break;
						}
					}
					if (client_index == -1)
					{
						std::cerr << "Error: could not find client socket." << std::endl;
						continue;
					}
					char	buffer[1024];
					memset(buffer, 0, 1024);
					size_t bytes_received = recv(pollfds[i].fd, buffer, 1024, 0);
					if (bytes_received <= 0)
					{
						if (bytes_received == 0)
							std::cout << "Client disconnected." << std::endl;
						else
							std::cerr << "Error: could not receive data from client." << std::endl;
						close(pollfds[i].fd);
						pollfds[i].fd = -1;
						clients.erase(clients.begin() + client_index);
					}
					else
					{
						std::cout << "Received " << bytes_received << " bytes from client: " << buffer << std::endl;
						size_t bytes_sent = send(pollfds[i].fd, buffer, bytes_received, 0);
						if (bytes_sent == -1)
							std::cerr << "Error: could not send data to client." << std::endl;
					}
				}
			}
		}
	}
	return (EXIT_SUCCESS);
}
