#include "../include/webserv.hpp"

volatile sig_atomic_t sigInt = 0;

int main(int argc, char** argv)
{
	ConfigFile configfile(pathConfigFile(argc, argv));
	std::vector<Server>& servers = configfile.getServers();
	std::vector<pollfd> pollVector;
	
	std::signal(SIGINT, sigHandler);
	launchServers(servers, pollVector);
	
	while (poll_(pollVector))
	{
		try
		{
			size_t i = 0;
			for (; i < servers.size(); ++i)
			{
				if (pollVector[i].revents & POLLIN) // risky? but should be correct
					servers[i].acceptConnections();
			}
			for (; i < pollVector.size(); ++i)
			{
				if (pollVector[i].revents & POLLHUP)
					
				if (pollVector[i].revents & POLLIN)

			}



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

const char*	pathConfigFile(int argc, char** argv)
{
	std::string path = "system/configs/example.conf";
	if (argc > 1)
		path = argv[1];
	return path.c_str();
}

void launchServers(std::vector<Server>& servers, std::vector<pollfd>& pollVector)
{
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
}

bool poll_(std::vector<pollfd>& pollVector)
{
	ANNOUNCEME
	if (poll(pollVector.data(), pollVector.size(), -1) == -1)
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
	if (sig == SIGINT) // can only be SIGINT but nicer than voiding it
		sigInt = 1;
}

void shutdown(std::vector<pollfd>& pollVector)
{
	std::cout << "\nShutdown." << std::endl;
	
	for (std::vector<pollfd>::iterator it = pollVector.begin(); it != pollVector.end(); ++it)
		close(it->fd);
}
