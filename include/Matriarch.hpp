#ifndef MATRIARCH_HPP
# define MATRIARCH_HPP

# include "webserv.hpp"

class	Matriarch
{
	public:
		Matriarch(int, char**);
		~Matriarch();

		void launchServers();
		bool poll();
		void acceptClients();







	private:
		void handleClients();
		void closeClient(std::string);
		bool receive();
		bool requestHead();

		//utils
		std::vector<Client>::iterator	getClient(int);
		std::vector<pollfd>::iterator	getPollStruct(int fd);
		void							addPollStruct(int fd, short flags);



		ConfigFile*				_configFile;
		std::vector<pollfd> 	_pollVector;
		std::vector<Server>		_servers;
		std::vector<Client>		_clientVector;
		std::vector<Client>::iterator	_client;
		std::vector<pollfd>::iterator	_pollStruct;
};

#endif
