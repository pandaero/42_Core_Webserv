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
		for (size_t i = 0; i < servers.size(); ++i)
		{
			try
			{
				servers[i].acceptConnections();
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

void sigHandler(int sig)
{
	if (sig == SIGINT)
	{
		std::cout << "\nShutdown." << std::endl;
		sigInt = 1;
	}
}
