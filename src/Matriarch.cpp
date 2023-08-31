#include "../include/Matriarch.hpp"

Matriarch::Matriarch(int argc, char** argv): _configFile(NULL), _servers(NULL)
{
	std::string path = "system/configs/example.conf";
	if (argc > 1)
		path = argv[1];
	_configFile = new ConfigFile(path.c_str());
	_servers = _configFile->getServers(); // maybe elegantize this later

}

Matriarch::~Matriarch()
{
	if (_configFile)
		delete _configFile;
}

void Matriarch::launchServers()
{
	for (size_t i = 0; i < _servers.size(); ++i)
	{
		try
		{
			_servers[i].whoIsI();
			_servers[i].startListening(_pollVector); // remove from server, include in matriarch
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			_servers.erase(_servers.begin() + i--);
		}
	}
}

bool Matriarch::poll()
{
	if (::poll(_pollVector.data(), _pollVector.size(), -1) == -1)
	{
		if (!sigInt)
			std::cerr << E_POLL;
	}
	if (sigInt)
		return false;
	return true;
}

void Matriarch::acceptClients()
{
	for (size_t i = 0; i < _servers.size(); ++i)
	{
		if (_servers[i].fd() != _pollVector[i].fd)
			std::cout << MMMMMEGAERROR << std::endl;
		
		if (!(_pollVector[i].revents & POLLIN))
			continue;
			
		while (true)
		{
			sockaddr_in	addr;
			socklen_t	clientAddrSize = sizeof(addr);
		
			int new_sock = accept(_pollVector[i].fd, (sockaddr*)&addr, &clientAddrSize);
			if (new_sock == -1)
			{
				if (errno == EWOULDBLOCK)
					return;
				closeFdAndThrow(new_sock);
			}
			if (fcntl(new_sock, F_SETFL, O_NONBLOCK) == -1)
				closeFdAndThrow(new_sock);

			addPollStruct(new_sock, POLLIN | POLLHUP);
			_clientVector.push_back(Client(_servers[i].getConfig(), _pollVector.back(), addr));
		}
	}
}

void Matriarch::handleClients()
{
	size_t i = _servers.size();
	while (i < _pollVector.size())
	{
		_client = getClient(_pollVector[i].fd);
		_pollStruct = getPollStruct(_pollVector[i].fd); // pollvectorbegin + i

		if (_pollStruct->revents & POLLHUP)
		{
			closeClient(CLOSEPOLLHUP);
			continue;
		}

		if (_pollStruct->revents & POLLIN)
		{
			if (!receive())
				continue;
			_client->incomingData();
		}

		if (_pollStruct->revents & POLLOUT)
		{
			if (!send())
				continue;
			_client->outgoingData();
		}
		++i;
	}
}


void Matriarch::closeClient(std::string msg)
{
	if (msg.empty())
		msg = "no information given.";
	std::cout << "closeClient on fd " << _client->fd << ": " << msg << std::endl;
	
	_pollVector.erase(_pollStruct);
	_clientVector.erase(_client);
}

clientVec_it Matriarch::getClient(int fd)
{
	clientVec_it it = _clientVector.begin();
	
	while (it != _clientVector.end() && it->getFd() != fd)
		++it;
	if (it == _clientVector.end())
	{
		std::cerr << E_M_CLIENTNOTFOUND << std::endl;
		throw std::runtime_error(__FUNCTION__);
	}
	return it;
}

std::vector<pollfd>::iterator Matriarch::getPollStruct(int fd)
{
	std::vector<pollfd>::iterator it = _pollVector.begin();
		
	while (it != _pollVector.end() && it->fd != fd)
		++it;
	if (it == _pollVector.end())
	{
		std::cerr << E_M_PSTRUCNOTFOUND << std::endl;
		throw std::runtime_error(__FUNCTION__);
	}
	return it;
}

bool Matriarch::receive()
{
	char buffer[RECV_CHUNK_SIZE];
	int bytesReceived = recv(_pollStruct->fd, buffer, RECV_CHUNK_SIZE, 0);
	if (bytesReceived <= 0)
	{
		closeClient(CLOSENODATA);
		return false;
	}
	_client->appendToBuf(buffer, bytesReceived);
	return true;
}

bool Matriarch::send()
{
	if (::send(_pollStruct->fd, _client->dataToSend().c_str(), _client->dataToSend().size(), 0) <= 0)
	{
		closeClient(E_M_SEND);
		return false;
	}
	return true;
}


void Matriarch::addPollStruct(int fd, short flags)
{
	pollfd new_pollStruct;
	new_pollStruct.fd = fd;
	new_pollStruct.events = flags;
	new_pollStruct.revents = 0;
	_pollVector.push_back(new_pollStruct);
}

void Matriarch::shutdown()
{
	std::cout << "\nShutdown." << std::endl;
	
	for (std::vector<pollfd>::iterator it = _pollVector.begin(); it != _pollVector.end(); ++it)
		close(it->fd);
}