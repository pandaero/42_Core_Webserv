#include "../include/Server.hpp"
#include "../include/Config.hpp"
#include "../include/Utils.hpp"

typedef std::vector<Server>::iterator ServerVecIt;



/* int main()
{
	Server	servster;

	
	std::string requestString("POST /api/users HTTP/1.1\r\nHost: www.example.com\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.96 Safari/537.36\nContent-Type: application/json\nContent-Length: 44\nConnection: keep-alive\n\n{\"username}\": \"jdoe\", \"password\": \"password123\"}");
	std::string empty("");
	Server serverA;

	serverA.parseRequest(empty);
	serverA.printRequest();
	serverA.parseRequest(requestString);
	serverA.printRequest();

	int i = 0;
	while (1)
	{
		//printf("\n+++++++ Waiting for new connection ++++++++\n\n");
		servster.poll();
		int newConnFd = servster.acceptConnection();
		char buffer[30000] = {0};
		read(newConnFd, buffer, 30000);
		printf("%s\n",buffer );
		

		write(newConnFd, msg, strlen(msg));
		printf("------------------Hello message sent-------------------\n");
		std::cout << "Number of connections: " << ++i << std::endl;
	}
} */




int main()
{
	std::vector<ServerConfig>	configVec;

	configVec = getConfigs("config/ngnix_style.conf");
	
	Server*	serverArr[configVec.size()];
	
	for (size_t i = 0; i < configVec.size(); i++)
	{
		serverArr[i] = new Server(configVec[i]);
		serverArr[i]->whoIsI();
	}

/* 	while (1)
	{
		for (ServerVecIt it = servers.begin(); it != servers.end(); it++)
			it->poll();
	} */
}
