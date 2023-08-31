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
	}
	webmother.shutdown();
}



void sigHandler(int sig)
{
	if (sig == SIGINT) // can only be SIGINT but nicer than voiding it
		sigInt = 1;
}


