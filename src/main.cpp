#include "../include/webserv.hpp"

int main()
{
	ConfigFile			configfile("default/config/ideal.conf");
	std::vector<Server>	servers(configfile.getServers());
	
	for (size_t i = 0; i < servers.size(); ++i)
	{
		try
		{
			servers[i].whoIsI();
			servers[i].startListening();
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			servers.erase(servers.begin() + i--);
		}
	}
	
	while (true)
	{
		for (size_t i = 0; i < servers.size(); ++i)
		{
			std::cout << "--- Handling Server index " << i << " ---" << std::endl;
			try
			{
				servers[i].poll();
				servers[i].acceptConnections();
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
