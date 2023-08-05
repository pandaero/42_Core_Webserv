/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 17:05:35 by pandalaf          #+#    #+#             */
/*   Updated: 2023/08/05 10:38:54 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Response.hpp"

Response::Response(){}

Response::Response(int code)
{
	std::string			httpMsg(getHttpMsg(code));
	std::stringstream	ss_body, ss_header;

	ss_body << "<html>\r\n<head><title>" << code << ": " << httpMsg << "</title></head>\r\n";
	ss_body << "<body style = \"text-align: left;\">\r\n<h1>" << code << " " << httpMsg << "</h1>\r\n";
	ss_body << "<img style=\"margin-left: auto;\", src=\"https://http.cat/" << code << "\">";
	ss_body << "\r\n</body>\r\n</html>\r\n";
	
	ss_header << HTTPVERSION << ' ' << code << httpMsg << "\r\n";
	ss_header << "Server: shmangserv/0.69 (knudel)" << "\r\n";
	ss_header << "content-type: text/html; charset=utf-8" << "\r\n";
	ss_header << "content-length: " << ss_body.str().size() << "\r\n";
	ss_header << "\r\n";

	_statusPage = ss_header.str() + ss_body.str();
}

Response::Response(const RequestHead & request, const Server & server)
{
	//DEBUG
	ANNOUNCEME
	std::cout << "Response getting made" << std::endl;
	
	// getPath depends on directory listing, and server root, etc. getFile?
	setFile(request.getPath(), server);

	// _statusCode = setFile(server.getRoot() + request.getPath(), server);
	// setStatusCode(_statusCode);
	// check for file access success etc.
	std::cout << "Response got made with full path: " << _filePath << std::endl;
}

//BULLSHIT CONSTRUCTOR? - set file to error page, perhaps
// Response::Response(int code, const Server & server)
// {
// 	setStatusCode(code);
// 	setFile(server.getStatusPage(code), server);
// }

Response::~Response(){}

const char* Response::getStatusPage()
{
	return _statusPage.c_str();
}

off_t Response::getSize()
{
	return _fileSize;
}

void	Response::setFile(std::string locationPath, const Server & currentServer)
{
	// Map location / URL to folder structure
	std::string	realPath = currentServer.getRoot() + locationPath;
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
		_filePath = currentServer.getStatusPage(404);
		_contentType = extensionType(_filePath);
		_fileSize = fileSize(_filePath);
		std::cerr << "Error page path: " << _filePath << std::endl;
		//setStatusCode(404);
		return;
	}
	_filePath = realPath;
	std::ifstream	file(_filePath.c_str(), std::ios::binary);
	if (!file)
	{
		std::cerr << "Error: Response: set: could not open file.";
		_filePath = currentServer.getStatusPage(500);
		_contentType = extensionType(_filePath);
		_fileSize = fileSize(_filePath);
		//setStatusCode(500);
		return;
	}
	// Determine size (find correct method)
	_fileSize = fileSize(realPath);
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
		headerStream << "\r\n" << "Content-Length: " << _fileSize << "\r\n\r\n";
	}
	headerStream << "\r\n\r\n";
	_responseHeader = headerStream.str();
}

int	Response::send(int socketfd, const Server & sendingServer)
{
	int headerBytesSent = 0;
	int	fileBytesSent = 0;
	int	closingBytesSent = 0;

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
		file.open(sendingServer.getStatusPage(_statusCode).c_str(), std::ios::binary);
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

std::string Response::getHttpMsg(int code)
{
	switch (code)
	{
		case 100: return "Continue\r\n";
		case 101: return "Switching Protocols\r\n";
		case 102: return "Processing\r\n";
		case 103: return "Checkpoint\r\n";

		case 200: return "OK\r\n";
		case 201: return "Created\r\n";
		case 202: return "Accepted\r\n";
		case 203: return "Non-Authoritative Information\r\n";
		case 204: return "No Content\r\n";
		case 205: return "Reset Content\r\n";
		case 206: return "Partial Content\r\n";
		case 207: return "Multi-Status\r\n";
		case 208: return "Already Reported\r\n";

		case 300: return "Multiple Choices\r\n";
		case 301: return "Moved Permanently\r\n";
		case 302: return "Found\r\n";
		case 303: return "See Other\r\n";
		case 304: return "Not Modified\r\n";
		case 305: return "Use Proxy\r\n";
		case 306: return "Switch Proxy\r\n";
		case 307: return "Temporary Redirect\r\n";
		case 308: return "Permanent Redirect\r\n";

		case 400: return "Bad Request\r\n";
		case 401: return "Unauthorized\r\n";
		case 402: return "Payment Required\r\n";
		case 403: return "Forbidden\r\n";
		case 404: return "Not Found\r\n";
		case 405: return "Method Not Allowed\r\n";
		case 406: return "Not Acceptable\r\n";
		case 407: return "Proxy Authentication Required\r\n";
		case 408: return "Request Timeout\r\n";
		case 409: return "Conflict\r\n";
		case 410: return "Gone\r\n";
		case 411: return "Length Required\r\n";
		case 412: return "Precondition Failed\r\n";
		case 413: return "Payload Too Large\r\n";
		case 414: return "URI Too Long\r\n";
		case 415: return "Unsupported Media Type\r\n";
		case 416: return "Requested Range Not Satisfiable\r\n";
		case 417: return "Expectation Failed\r\n";
		case 418: return "I'm a teapot\r\n";
		case 421: return "Misdirected Request\r\n";
		case 422: return "Unprocessable Entity\r\n";
		case 423: return "Locked\r\n";
		case 424: return "Failed Dependency\r\n";
		case 426: return "Upgrade Required\r\n";
		case 428: return "Precondition Required\r\n";
		case 429: return "Too Many Request\r\n";
		case 431: return "Request Header Fields Too Large\r\n";
		case 451: return "Unavailable For Legal Reasons\r\n";

		case 500: return "Internal Server Error\r\n";
		case 501: return "Not Implemented\r\n";
		case 502: return "Bad Gateway\r\n";
		case 503: return "Service Unavailable\r\n";
		case 504: return "Gateway Timeout\r\n";
		case 505: return "HTTP Version Not Supported\r\n";
		case 506: return "Variant Also Negotiates\r\n";
		case 507: return "Insufficient Storage\r\n";
		case 508: return "Loop Detected\r\n";
		case 510: return "Not Extended\r\n";
		case 511: return "Network Authentication Required\r\n";

		default: return "Unknown Error\r\n";
	}
}

