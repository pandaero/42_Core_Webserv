/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 17:05:35 by pandalaf          #+#    #+#             */
/*   Updated: 2023/05/13 00:04:45 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Response.hpp"

Response::Response()
{

}

Response::Response(const Request & request, const Server & server)
{
	//DEBUG
	std::cout << "Response getting made" << std::endl;
	_statusCode = setFile(server.getRoot() + request.getPath());
	setStatusCode(_statusCode);
	// check for file access success etc.
	std::cout << "Response got made with full path: " << server.getRoot() + request.getPath() << std::endl;
}

//BULLSHIT CONSTRUCTOR? - set file to error page, perhaps
Response::Response(int code, const Server & server)
{
	setStatusCode(code);
	setFile(server.getStatusPage(code));
}

Response::~Response()
{

}

void	Response::setStatusCode(int code)
{
	_statusCode = code;

	switch (code)
	{
		case 200:
			_statusMessage = "OK";
			break;
		case 404:
			_statusMessage = "Not Found";
			break;
		case 500:
			_statusMessage = "Internal Server Error";
			break;
		default:
			_statusMessage = "Internal Server Error";
			break;
	}
}

int	Response::setFile(std::string filePath)
{
	// If SCHMANG happens, set to corresponding error code
	int	ret = 200;
	_contentType = extensionType(filePath);
	// DEBUG
	std::cout << "Opening: " << filePath << std::endl;
	int acc = access(filePath.c_str(), F_OK);
	if (acc)
	{
		ret = 404;
		return (ret);
	}
	_filePath = filePath;
	FILE *	file = std::fopen(filePath.c_str(), "rb");
	if (!file)
	{
		std::cerr << "Error: Response: set: could not open file.";
		setStatusCode(500);
	}
	std::fseek(file, 0, SEEK_END);
	// Determine size (find correct method)
	_fileSize = std::ftell(file);
	std::fclose(file);
	return (ret);
}

void	Response::build()
{
	std::stringstream	headerStream;

	headerStream << "HTTP/1.1 " << _statusCode << " " << _statusMessage << "\r\n" << "Content-Length: 325";

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

int	Response::send(int socketfd)
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
	if (_statusCode != 404)
	{
		// Open file and send contents through socket
		file.open(_filePath.c_str(), std::ios::binary);
	}
	else
	{
		// Set corresponding error file, need file-path map from server
		file.open("default/error/404.html", std::ios::binary);
	}
	if (file.fail())
	{
		std::cerr << "Error: Response: send: could not open file.";
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
