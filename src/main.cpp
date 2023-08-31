#include "../include/webserv.hpp"

int main(int argc, char** argv)
{
	sigInt = 0;
	std::signal(SIGINT, sigHandler);
	
	Matriarch webmother(argc, argv);
	
	webmother.launchServers();
	
	while (webmother.poll())
	{
		try
		{
			webmother.acceptClients();
			



		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}

		
		
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
	shutdown(pollVector);
}



void sigHandler(int sig)
{
	if (sig == SIGINT) // can only be SIGINT but nicer than voiding it
		sigInt = 1;
}

void shutdown(std::vector<pollfd>& pollVector)
{
	std::cout << "\nShutdown." << std::endl;
	
	for (std::vector<pollfd>::iterator it = pollVector.begin(); it != pollVector.end(); ++it)
		close(it->fd);
}
