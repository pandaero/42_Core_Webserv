#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <cstring>
#incldue <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>


int main()
{
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        std::cerr << "Error: could not create server socket." << std::endl;
        return (EXIT_FAILURE);
    }

    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(12345);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (sockaddr*)&server_address, sizeof(server_address)) == -1)
    {
        std::cerr << "Error: could not bind server socket to port." << std::endl;
        close(server_socket);
        return (EXIT_FAILURE);
    }

    if (listen(server_socket, SOMAXCONN) == -1)
    {
        std::cerr << "Error: could not start listening on server socket." << std::endl;
        close(server_socket);
        return (EXIT_FAILURE);
    }

    std::vector<int> clients;
    fd_set readfds;
    int max_sd, activity;

    while (true)
    {
        FD_ZERO(&readfds);
        FD_SET(server_socket, &readfds);
        max_sd = server_socket;

        for (auto const& client : clients)
        {
            FD_SET(client, &readfds);
            if (client > max_sd)
            {
                max_sd = client;
            }
        }

        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if ((activity < 0) && (errno != EINTR))
        {
            std::cerr << "Error: could not start select on server socket." << std::endl;
            close(server_socket);
            return (EXIT_FAILURE);
        }

        if (FD_ISSET(server_socket, &readfds))
        {
            int client_socket = accept(server_socket, NULL, NULL);
            if (client_socket == -1)
            {
                std::cerr << "Error: could not accept incoming connection." << std::endl;
            }
            else
            {
                std::cout << "New client connected." << std::endl;
                clients.push_back(client_socket);
            }
        }

        for (auto const& client : clients)
        {
            if (FD_ISSET(client, &readfds))
            {
                char buffer[1024] = {0};
                int read_size = read(client, buffer, 1024);
                if (read_size == -1)
                {
                    std::cerr << "Error: could not read data from client socket." << std::endl;
                }
                else if (read_size == 0)
                {
                    std::cout << "Client disconnected." << std::endl;
                    close(client);
                    clients.erase(std::remove(clients.begin(), clients.end(), client), clients.end());
                }
                else
                {
                    std::cout << "Received data from client: " << buffer << std::endl;
                    if (send(client, buffer, strlen(buffer), 0) == -1)
                    {
                        std::cerr << "Error: could not send data to client." << std::endl;
                    }
                }
            }
        }
    }

    close(server_socket);
    return (EXIT_SUCCESS);
}
