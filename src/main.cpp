#include "../include/webserv.hpp"

volatile sig_atomic_t sigInt = 0;

int main()
{
	ConfigFile configfile("default/config/example.conf");
	std::vector<Server>& servers = configfile.getServers();
	std::vector<pollfd> pollVector;
	
	std::signal(SIGINT, sigHandler);

	for (size_t i = 0; i < servers.size(); ++i)
	{
		try
		{
			servers[i].whoIsI();
			servers[i].startListening(pollVector);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			servers.erase(servers.begin() + i--);
		}
	}
	
	while (poll_(pollVector))
	{
		try
		{
			acceptConnections(servers, pollVector);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	
		for (size_t i = 0; i < servers.size(); ++i)
		{
			try
			{
				servers[i].handleConnections();
			}
			catch (const std::exception& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}

	}
}

bool poll_(std::vector<pollfd>& pollVector)
{
	ANNOUNCEME
	if (poll(&pollVector[0], pollVector.size(), -1) == -1)
	{
		if (!sigInt)
			std::cerr << E_POLL;
	}
	if (sigInt)
		return false;
	return true;
}

/* 	
currently not checking for a size restriction. Revisit this during testing.
prolly just returns from accept with -1. 
if (pollVector.size() > 420)
	{
		std::cerr << I_CONNECTIONLIMIT << std::endl;
		return;
	}
*/
void acceptConnections(std::vector<Server>& servers, std::vector<pollfd>& pollVector)
{
	ANNOUNCEME
	for (size_t i = 0; i < servers.size(); ++i)
	{
		std::cout << "server_fd:" << servers[i].fd() << ". pollvector[i].fd:" << pollVector[i].fd << "." << std::endl;
		if (!(pollVector[i].revents & POLLIN))
			continue;
		while (true)
		{
			int new_sock = accept(servers[i].fd(), NULL, NULL); // don't need client info, so pass NULL
			if (new_sock == -1)
			{
				if (errno != EWOULDBLOCK)
					std::cerr << E_ACCEPT << std::endl;
				break;
			}
			int flags = fcntl(new_sock, F_GETFL, 0);
			if (fcntl(new_sock, F_SETFL, flags | O_NONBLOCK) == -1)
				closeAndThrow(new_sock);
			servers[i].addClient(new_sock);
			
			pollfd new_pollStruct;
			new_pollStruct.fd = new_sock;
			new_pollStruct.events = POLLIN | POLLOUT | POLLHUP;
			new_pollStruct.revents = 0;
			pollVector.push_back(new_pollStruct);
			std::cout << "New client accepted for servlet #" << i << " on fd " << new_sock << "." << std::endl;
			std::cout << "PollStructs in Vector: " << pollVector.size() << std::endl;
		}
	}
}

void sigHandler(int sig)
{
	if (sig == SIGINT)
	{
		std::cout << "\nShutdown." << std::endl;
		sigInt = 1;
	}
}
