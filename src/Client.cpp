#include "../include/Client.hpp"

Client::Client(const ServerConfig& config, pollfd& pollStruct, sockaddr_in addr):
	_config(config), _activeConfig(&config), _pollStruct(pollStruct), _fd(pollStruct.fd), 
	_address(addr), _statusCode(0), _filePosition(0), _bytesWritten(0), _contentLength(0),
	_state(recv_reqHead), _dirListing(false), _append(false), _setCookie(false),
	_childLaunched(false), _cgiRequest(false)
{}

void Client::incomingData()
{
	// requesthead incomplete (only possible on first run, would throw error if not done then)
	// requesthead complete - body incomplete (only possible on post on any run)
	// requestehad complete - body complete (possible on all if immediate body completion)
	if (_state == recv_reqHead) //can be a bool so far but is ok i guess?
	{
		requestHead();
		requestHeadError();
		if (_statusCode != 0)
			return;
		isCgiRequest();
		whoIsI();
		if (_method == GET)
		{
			if (_cgiRequest)
				handleGetCGI(); // shmangidy
			else
				handleGet();
		}
		else if (_method == DELETE)
			handleDelete();
		else if (_method == POST)
		{
			_state = recv_reqBody;
			if (_cgiRequest)
				handlePostCGI(); // shmangidy
			else
				handlePost();
		}
	}
	else // remaining case is POST body not yet complete
	{
		if (_method != POST || _state != recv_reqBody)
			std::cout << MMMMMEGAERROR << std::endl;
		if (_cgiRequest)
			handlePostCGI();
		else
			handlePost();
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
	_state = send_respHead;
	_pollStruct.events = POLLOUT | POLLHUP;

	if (_activeConfig->getStatusPagePaths().find(code) == _activeConfig->getStatusPagePaths().end())
	{
		generateStatusPage(code);
		return;
	}
	std::string path = prependRoot(_activeConfig->getStatusPagePaths()[code]);
	if (resourceExists(path))
	{
		_sendPath = path;
		_sendFile = true;
	}
	else
		generateStatusPage(code);
	outgoingData();
}

void Client::sendFile_200(std::string sendPath)
{
	_statusCode = 200;
	_state = send_respHead;
	_pollStruct.events = POLLOUT | POLLHUP;
	_sendPath = sendPath;
	_sendFile = true;
	outgoingData();
}

void Client::sendEmptyStatus(int code)
{
	_statusCode = code;
	_state = send_respHead;
	_pollStruct.events = POLLOUT | POLLHUP;
	_sendPath.clear(); // should be empty anyway, but structural symmetry
	_sendFile = true; // will not do anything cause empty sendPath
	outgoingData();
}

std::string Client::dataToSend()
{
	return _sendStream.str();
}

void Client::outgoingData()
{
	if (_state == send_respHead)
	{
		// if cgi child finished?
		_sendStream << buildResponseHead();
		_state = send_respBody;
		return;
	}
	if (_state == send_respBody)
	{
		if (_sendFile)
		{
			// send sendPath
		}
		else
		{
			char buffer[SEND_CHUNK_SIZE];
			_pageBuffer.read(buffer, SEND_CHUNK_SIZE);
			_sendStream.write(buffer, _pageBuffer.gcount());
			// if buffer is not done, gotta get rid of read shit
		}


	}
}

std::string Client::buildResponseHead()
{
	std::stringstream ss_header;
	size_t contentLength = fileSize(_sendPath);
	
	ss_header		<< HTTPVERSION << ' ' << _statusCode << ' ' << getHttpMsg(_statusCode) << "\r\n"
					<< "Server: " << SERVERVERSION << "\r\n"
					<< "connection: close" << "\r\n";
	if (_cgiRequest)
		ss_header	<< "transfer-encoding: chunked\r\n"
					<< "content-type: " << mimeType(".html") << "\r\n"; // we only return html when it is a CGI request 
	else
	{
		ss_header << "content-length: " << contentLength << "\r\n";
		if (contentLength != 0)
			ss_header << "content-type: " << mimeType(_sendPath) << "\r\n";
	}
	if (_setCookie)
		ss_header << cookie(SESSIONID, _sessionId, 3600, "/") << "\r\n";
	ss_header << "\r\n";
	return ss_header.str();
}

std::string Client::cookie(const std::string& key, const std::string& value, int expiration, const std::string& path)
{
	std::stringstream cookie;
	cookie << "set-cookie: " << key << "=" << value << ";";
	if (expiration >= 0)
		cookie << "max-age=" << expiration << ";";
	cookie << "path=" << path << ";";
	return cookie.str();
}


bool Client::responseHead()
{
	if (_clientIt->state > send_respHead)
		return true;
	ANNOUNCEME_FD

	if (_clientIt->cgiRequest)
	{
		if (!childFinished())
			return false;
	}

	std::string head = buildResponseHead();

	if (send(_clientIt->fd, head.c_str(), head.size(), 0) <= 0)
	{
		closeClient("Server::sendResponseHead: send failure.");
		throw std::runtime_error(E_SEND);
	}
	std::cout << "responseHead sent to fd: " << _clientIt->fd << "\n" << head << std::endl;
	_clientIt->state = send_respBody;
	return false;
}