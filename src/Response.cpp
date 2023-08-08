#include "../include/Response.hpp"

Response::Response(){}

Response::~Response(){}

Response::Response(int code, strVec serverNames)
{
	(void)serverNames;
	std::string			httpMsg(getHttpMsg(code));
	std::stringstream	ss_body, ss_header;

	ss_body << "<!DOCTYPE html>";
	ss_body << "<html>\n";
	ss_body << "<head>\n";
	ss_body << "<title>webserv - " << code << ": " << httpMsg << "</title>\n";
	ss_body << "<style>\n";
	ss_body << "body {background-color: black; color: white; font-family: Arial, sans-serif; margin: 0; padding: 5% 0 0 0; text-align: center;}\n";
	ss_body << "h1 {font-size: 42px;}\n";
	ss_body << "p {font-size: 16px; line-height: 1.5;}\n";
	ss_body << "</style>\n";
	ss_body << "</head>\n";
	ss_body << "<body>\n";
	ss_body << "<h1>" << code << ": " << httpMsg << "</h1>\n";
	ss_body << "<img style=\"margin-left: auto;\" src=\"https://http.cat/" << code << "\" alt=\"" << httpMsg << "\">\n";
	ss_body << "</body>\n";
	ss_body << "</html>\n";
	
	ss_header << HTTPVERSION << ' ' << code << ' ' << httpMsg << "\r\n";
	ss_header << "Server: " << SERVERVERSION << "\r\n";
	ss_header << "content-type: text/html; charset=utf-8" << "\r\n";
	ss_header << "content-length: " << ss_body.str().size() << "\r\n";
	ss_header << "\r\n";

	_statusPage = ss_header.str() + ss_body.str();
	_bodySize = _statusPage.size();
}

Response::Response(const Request & request, const Server & server)
{
	//DEBUG
	ANNOUNCEME
	std::cout << "Response getting made" << std::endl;
	
	// getPath depends on directory listing, and server root, etc. getFile?
	setFile(request.path(), server);

	// _statusCode = setFile(server.getRoot() + request.path(), server);
	// setStatusCode(_statusCode);
	// check for file access success etc.
	std::cout << "Response got made with full path: " << _filePath << std::endl;
}

const char* Response::getStatusPage()
{
	return _statusPage.c_str();
}

off_t Response::getSize()
{
	return _bodySize;
}


void	Response::setFile(std::string locationPath, const Server & currentServer)
{
	// Map location / URL to folder structure
	(void)currentServer;
	std::string	realPath = "currentServer" + locationPath;
	size_t	indexLastChar = realPath.length() - 1;
	if (realPath[indexLastChar] == '/')
		realPath.erase(indexLastChar); // are there edge cases? like just pointing to root? "/"
	/* if (realPath.find_last_of('/') == realPath.size() - 1)
		realPath.erase(realPath.find_last_of('/')); */
	// If path is a directory, search for index.html. If not found, check for dir listing 
	// Consider dir listing in case of dir and serve corresponding (created) dir listing
	// If SCHMANG happens, set to corresponding error code
	// DEBUG
	std::cout << "Opening: " << realPath << std::endl;
	int acc = access(realPath.c_str(), F_OK);
	if (acc)
	{
		//_filePath = currentServer.getStatusPage(404);
		_contentType = extensionType(_filePath);
		_bodySize = fileSize(_filePath);
		std::cerr << "Error page path: " << _filePath << std::endl;
		//setStatusCode(404);
		return;
	}
	_filePath = realPath;
	std::ifstream	file(_filePath.c_str(), std::ios::binary);
	if (!file)
	{
		std::cerr << "Error: Response: set: could not open file.";
		//_filePath = currentServer.getStatusPage(500);
		_contentType = extensionType(_filePath);
		_bodySize = fileSize(_filePath);
		//setStatusCode(500);
		return;
	}
	// Determine size (find correct method)
	_bodySize = fileSize(realPath);
	_contentType = extensionType(realPath);
	//setStatusCode(200);
	return;
}


void	Response::build()
{
	std::stringstream	headerStream;

	headerStream << "HTTP/1.1 " << _statusCode << " " << _statusMessage << "\r\n";

	if (!_filePath.empty())
	{
		headerStream << "Content-Type: ";
		switch (_contentType)
		{
			case PLAINTEXT:
				headerStream << "text/plain";
				break;
			case HTML:
				headerStream << "text/html";
				break;
			case CSS:
				headerStream << "text/css";
				break;
			case ZIP:
				headerStream << "application/zip";
				break;
			case PNG:
				headerStream << "image/png";
				break;
			case JPEG:
				headerStream << "image/jpeg";
				break;
			case PDF:
				headerStream << "application/pdf";
				break;
			case XML:
				headerStream << "application/xml";
				break;
			case JSON:
				headerStream << "application/json";
				break;
			case AVIF:
				headerStream << "image/avif";
				break;
			default:
				headerStream << "application/octet-stream";
		}
		headerStream << "\r\n" << "Content-Length: " << _bodySize << "\r\n\r\n";
	}
	headerStream << "\r\n\r\n";
	_responseHeader = headerStream.str();
}

int	Response::send(int socketfd, const Server & sendingServer)
{
	int headerBytesSent = 0;
	int	fileBytesSent = 0;
	int	closingBytesSent = 0;
	
	(void)sendingServer;
	
	build();
	// DEBUG
	std::cout << "Header:\n" << _responseHeader << std::endl;
	// Send header
	if ((headerBytesSent += ::send(socketfd, _responseHeader.data(), _responseHeader.size(), 0)) == -1)
	{
		std::cerr << "Error: Response: send: failure to send header data.";
		return (-1);
	}
	std::ifstream	file;
	// Send file or corresponding status page.
	if (_statusCode == 200)
		file.open(_filePath.c_str(), std::ios::binary);
	else
		//file.open(sendingServer.getStatusPage(_statusCode).c_str(), std::ios::binary);
	if (file.fail())
	{
		std::cerr << "Error: Response: send: could not open file." << std::endl;
		return (-1);
	}
	char	buffer[1];
	// DEBUG
	std::cout << "sending file: " << _filePath << std::endl;
	while (file.read(buffer, sizeof(buffer)))
	{
		if ((fileBytesSent += ::send(socketfd, buffer, file.gcount(), 0)) == -1)
		{
			std::cerr << "Error: Response: send: could not send file data.";
			return (-1);
		}
		// DEBUG
		std::cout << "the buffer:\n" << buffer << std::endl;
	}
	// if (!file.eof())
	// {
	// 	std::cerr << "Error: Response: send: could not read entire file.";
	// 	return (-1);
	// }
	// DEBUG
	std::cout << "Reached EOF" << std::endl;
	file.close();
	// Send termination CRLFs
	std::string	terminationSequence(TERMINATION);
	if ((closingBytesSent += ::send(socketfd, terminationSequence.data(), terminationSequence.size(), 0)) == -1)
	{
		std::cerr << "Error: Response: send: failure to send termination data.";
		return (-1);
	}
	// DEBUG
	std::cout << "Response sent, " << fileBytesSent << " bytes from file." << std::endl;
	return (fileBytesSent);
}

std::string Response::getFilePath()
{
	return _filePath;
}
