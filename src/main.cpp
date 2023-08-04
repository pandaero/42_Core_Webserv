#include "../include/Server.hpp"
#include "../include/ServerConfig.hpp"
#include "../include/ConfigFile.hpp"
#include "../include/webserv.hpp"

int main()
{
	ConfigFile			configfile("default/config/ideal.conf");
	std::vector<Server>	servers;
	
	servers = configfile.getServers();
	
	for (size_t i = 0; i < servers.size(); ++i)
	{
		servers[i].whoIsI();
		servers[i].startListening();
	}
	
	while (true)
	{
		for (size_t i = 0; i < servers.size(); ++i)
		{
			std::cout << "Polling Server i: " << i << std::endl;
			try
			{
				servers[i].poll();
				// std::cout << "Try poll success.\n";
			}
			catch (std::exception & exc)
			{
				// std::cerr << "Error: could not poll on a socket." << std::endl;
				perror("pollerror");
				continue;
			}
			servers[i].handleConnections();
			std::cout << "Server i: " << i << std::endl;
		}
	}

	// I mean... How smart is it to put stuff behind a while true loop and no break?
	// still, this reminds us of stuff that should be freed on shutdown!
	for (size_t i = 0; i < servers.size(); ++i)
		servers[i].cleanup();
}
