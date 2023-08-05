#include "../include/Server.hpp"
#include "../include/ServerConfig.hpp"
#include "../include/ConfigFile.hpp"
#include "../include/webserv.hpp"

int main()
{
	ConfigFile			configfile("default/config/ideal.conf");
	std::vector<Server>	servers(configfile.getServers());
	
	for (size_t i = 0; i < servers.size(); ++i)
	{
		servers[i].whoIsI();
		servers[i].startListening();
	}
	
	while (true)
	{
		for (size_t i = 0; i < servers.size(); ++i)
		{
			std::cout << "--- Handling Server index " << i << " ---" << std::endl;
			try
			{
				servers[i].poll();
			}
			catch (std::exception & exc)
			{
				perror("poll");
				continue;
			}
			try
			{
				servers[i].acceptConnections();
			}
			catch (const std::exception& e)
			{
				std::cerr << e.what() << std::endl;
			}
			try
			{
				servers[i].checkConnections();
			}
			catch (const std::exception& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
	}

	// I mean... we are behind a while true loop with no break
	// still, this reminds us of stuff that should be freed on shutdown
	// maybe capture SIGINT and run this shite then if we rly want
	for (size_t i = 0; i < servers.size(); ++i)
		servers[i].cleanup();
}
