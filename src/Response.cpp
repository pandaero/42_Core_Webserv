/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pandalaf <pandalaf@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 17:05:35 by pandalaf          #+#    #+#             */
/*   Updated: 2023/05/14 18:38:40 by pandalaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Response.hpp"

Response::Response(){}

Response::Response(const Request & request, const Server & server)
{
	// DEBUG
	std::cout << "Response getting made" << std::endl;
	// Check method permissions at location and throw 403 if forbidden.
	// checkMethodAccess?
	setFile(request.getPath(), server);
	// DEBUG
	std::cout << "Response got made with full path: " << _filePath << std::endl;
}

//BULLSHIT CONSTRUCTOR? - set file to error page, perhaps
// Response::Response(int code, const Server & server)
// {
// 	setStatusCode(code);
// 	setFile(server.getStatusPage(code), server);
// }

Response::~Response(){}

void	Response::setStatusCode(int code)
{
	_statusCode = code;

	switch (code)
	{
		case 200:
			_statusMessage = "OK";
			break;
		case 403:
			_statusMessage = "Forbidden";
			break;
		case 404:
			_statusMessage = "Not Found";
			break;
		default:
			_statusMessage = "Internal Server Error";
	}
}

void	Response::setFile(std::string locationPath, const Server & currentServer)
{
	// Map location / URL to folder structure
	std::string	realPath = currentServer.getRoot() + locationPath;
	if (realPath.find_last_of('/') == realPath.size() - 1)
	{
		realPath.erase(realPath.find_last_of('/'));
		// Check for index files
		if (access((realPath + "/index.html").c_str(), R_OK) == 0)
			realPath += "/index.html";
		else if (access((realPath + "/index.htm").c_str(), R_OK) == 0)
			realPath += "/index.html";
		else
			realPath = currentServer.getRoot() + currentServer.getStatusPage(404);
		// Check for listing and create it to be served (or serve alternate landing page).
		// if listing true, create listing
		// if listing false, display page
		if (currentServer.getServerParam("dir_listing") == 1)
			std::cout << "Happy birthday" << std::endl;
	}
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
		setStatusCode(404);
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
		setStatusCode(500);
		return;
	}
	// Determine size (find correct method)
	_fileSize = fileSize(realPath);
	_contentType = extensionType(realPath);
	setStatusCode(200);
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
		// std::cout << "the buffer:\n" << buffer << std::endl;
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
