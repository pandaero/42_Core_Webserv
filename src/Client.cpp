#include "../include/Client.hpp"

Client::Client(const ServerConfig& config, pollfd& pollStruct, sockaddr_in addr):
	_config(config), _activeConfig(&config), _pollStruct(pollStruct), _fd(pollStruct.fd), 
	_address(addr), _statusCode(0), _filePosition(0), _bytesWritten(0), _contentLength(0),
	_state(recv_head), _dirListing(false), _append(false), _setCookie(false),
	_childLaunched(false), _cgiRequest(false)
{}

void Client::incomingData()
{
	// requesthead incomplete (only possible on first run, would throw error if not done then)
	// requesthead complete - body incomplete (only possible on post on any run)
	// requestehad complete - body complete (possible on all if immediate body completion)
	if (_state == recv_head) //can be a bool so far but is ok i guess?
	{
		requestHead();
		requestHeadError();
		if (_statusCode != 0)
			return;
		isCgiRequest();
		whoIsI();
		if (_method == GET)
			handleGet();
		else if (_method == DELETE)
			handleDelete();
		else if (_method == POST)
		{
			if (_contentLength <= _buffer.size())  // body is already complete
				//process POST
			else
				_state = recv_body;
		}
	}


}

int	Client::getFd()
{
	return _fd;
}

void Client::appendToBuf(char* buffer, int bytesRecvd)
{
	_buffer.append(buffer, bytesRecvd);
}

void Client::sendStatusPage(int code)
{
	_statusCode = code;
	_state = send_head;
	_pollStruct.events = POLLOUT | POLLHUP;

	if (_activeConfig->getStatusPagePaths().find(code) == _activeConfig->getStatusPagePaths().end())
	{
		generateStatusPage(code);
		return;
	}
	std::string path = prependRoot(_activeConfig->getStatusPagePaths()[code]);
	if (resourceExists(path))
		_sendPath = path;
	else
		generateStatusPage(code);
}

void Client::sendFile_200(std::string sendPath)
{
	_statusCode = 200;
	_state = send_head;
	_pollStruct.events = POLLOUT | POLLHUP;
	_sendPath = sendPath;
}

void Client::sendEmptyStatus(int code)
{
	_statusCode = code;
	_state = send_head;
	_pollStruct.events = POLLOUT | POLLHUP;
	_sendPath.clear(); // should be empty anyway, but structural symmetry
}
