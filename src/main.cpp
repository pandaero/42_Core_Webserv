#include "../include/Server.hpp"
#include "../include/ServerConfig.hpp"
#include "../include/webserv.hpp"

int main()
{
	std::vector<ServerConfig>	configVec;
	size_t						serverCount;

	configVec = getConfigs("default/config/nginx_style.conf");
	serverCount = configVec.size();

	Server *	serverArr[serverCount];
	for (size_t i = 0; i < serverCount; i++)
	{
		serverArr[i] = new Server(configVec[i]);
		serverArr[i]->whoIsI();
	}

	while (true)
	{
		for (size_t i = 0; i < serverCount; ++i)
		{
			try
			{
				serverArr[i]->poll();
			}
			catch (std::exception & exc)
			{
				// std::cerr << "Error: could not poll on a socket." << std::endl;
				perror("poll");
				continue;
			}
			serverArr[i]->handleConnections();
		}
	}
}
