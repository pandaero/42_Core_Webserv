#include "../include/webserv.hpp"

void acceptConnections(std::vector<Server>&, std::vector<pollfd>&);
void poll_(std::vector<pollfd>&);

int main()
{
	ConfigFile			configfile("default/config/ideal.conf");
	std::vector<Server>	servers(configfile.getServers());
	std::vector<pollfd>	pollVector;
	
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
	
	while (true)
	{
		poll_(pollVector);
		acceptConnections(servers, pollVector);
		
		for (size_t i = 0; i < servers.size(); ++i)
		{
			std::cout << "--- Handling servlet #" << i << " ---" << std::endl;
			try
			{
				servers[i].handleConnections();
			}
			catch (const std::exception& e)
			{
				std::cerr << e.what() << std::endl;
				std::cerr << "mainCatch" << std::endl;
			}
		}
	}
}

void poll_(std::vector<pollfd>& pollVector)
{
	if (poll(&pollVector[0], pollVector.size(), -1) == -1)
			std::cerr << E_POLL;
}


/* 	
currently not checking for a size restriction. Revisit this during testing.
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
