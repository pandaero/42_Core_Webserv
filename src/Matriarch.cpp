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
			std::cout << "Mismatch between server fd and pollvector fd. Returning from acceptConnections._____________________________________________________________________________________________________" std::endl;
		
		if (!(_pollVector[i].revents & POLLIN))
			continue;
			
		while (true)
		{
			Client newClient;
			socklen_t clientAddrSize = sizeof(newClient.address);
		
			int new_sock = accept(_pollVector[i].fd, (sockaddr*)&newClient.address, &clientAddrSize);
			if (new_sock == -1)
			{
				if (errno == EWOULDBLOCK)
					return;
				closeFdAndThrow(new_sock);
			}
			if (fcntl(new_sock, F_SETFL, O_NONBLOCK) == -1)
				closeFdAndThrow(new_sock);
			newClient.fd = new_sock;
			_clientVector.push_back(newClient);
			addPollStruct(new_sock, POLLIN | POLLHUP);
		}
	}
}

void Matriarch::handleClients()
{
	size_t i = _servers.size();
	while (i < _pollVector.size())
	{
		_client = getClient(_pollVector[i].fd);
		_pollStruct = getPollStruct(_pollVector[i].fd);

		if (_pollStruct->revents & POLLHUP)
		{
			closeClient(CLOSEPOLLHUP);
			continue;
		}

		if (_pollStruct->revents & POLLIN)
		{
			if (!receive())
				continue;
			if (_client->state == recv_head)
			{
				if (!requestHead())
				{
					++i;
					continue;
				}
			}

		}
		++i;
	}
}


bool Matriarch::requestHead()
{
	// basic format error: request line termination not found
	if (_client->buffer.find("\r\n") == std::string::npos)
	{
		
		sendStatusPage(400);
		return false;
	}
	
	parseRequestLine();
	
	// if buffer is empty, the request was only the request line. Not HTTP1.1 compliant, but we will accept it.
	if (!_clientIt->buffer.empty())
	{
		// check for proper headers termination
		if (_clientIt->buffer.find("\r\n\r\n") == std::string::npos)
		{
			if (_clientIt->buffer.size() >= MAX_HEADERSIZE) //This only makes sense if the readchunk is big enough.
				return (sendStatusPage(431), false);
			else
				return (sendStatusPage(400), false); // we read the entire "headers" but they weren't properly terminated.
		}
		parseRequestHeaders();
	}
	handleSession();
	if (requestError())
		return false;
	
	// check for body
	if (_clientIt->method != POST) // we don't process bodies of GET or DELETE requests
		_clientIt->state = handleRequest;
	else if (_clientIt->contentLength <= _clientIt->buffer.size()) // body is already complete in this recv (header content has already been deleted from buffer)
		_clientIt->state = handleRequest;
	else
		_clientIt->state = recv_body;

	selectHostConfig();
	updateClientVars();
	_clientIt->cgiRequest = cgiRequest();
	_clientIt->whoIsI();
	return true;
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
	
	while (it != _clientVector.end() && it->fd != fd)
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
	int bytesReceived = recv(_client->fd, buffer, RECV_CHUNK_SIZE, 0);
	if (bytesReceived <= 0)
	{
		closeClient(CLOSENODATA);
		return false;
	}
	_client->buffer.append(buffer, bytesReceived);
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