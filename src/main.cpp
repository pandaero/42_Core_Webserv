#include "../include/Server.hpp"
#include "../include/ServerConfig.hpp"
#include "../include/ConfigFile.hpp"
#include "../include/webserv.hpp"

/*
main to test ConfigFile object. Can be deleted. Kept it here in case 
you wanted to try shit out.
To read the ServerConfigs, use their member function whoIsi.
*/

/* 
int main()
{
	ConfigFile					configFile("default/config/site.conf");
	std::vector<ServerConfig>	configVec;

	configVec = configFile.getConfigs();

	configVec[1].whoIsI();


}
*/

int main()
{
	//std::vector<ServerConfig>	configVec;
	size_t						serverCount = 1;

	// configFile	config("");
	// configVec = config.getConfigs();

	// configVec = getConfigs("default/config/site.conf");
	// serverCount = configVec.size();

	Server *	serverArr[serverCount];
	// for (size_t i = 0; i < serverCount; i++)
	// {
	// 	serverArr[i] = new Server(configVec[i]);
	// 	serverArr[i]->whoIsI();
	// }

	ConfigFile	configFile("default/config/site.conf");

	serverArr[0] = new Server();

	serverArr[0]->whoIsI();

	while (true)
	{
	//	for (size_t i = 0; i < serverCount; ++i)
		size_t i = 0;
		{
			std::cout << "Polling Server i: " << i << std::endl;
			try
			{
				serverArr[i]->poll();
				std::cout << "Try poll success.\n";
			}
			catch (std::exception & exc)
			{
				// std::cerr << "Error: could not poll on a socket." << std::endl;
				perror("poll");
				continue;
			}
			serverArr[i]->handleConnections();
			std::cout << "Server i: " << i << std::endl;
		}
	}
}
