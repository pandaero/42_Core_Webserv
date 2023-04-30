/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pandalaf <pandalaf@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 17:05:35 by pandalaf          #+#    #+#             */
/*   Updated: 2023/04/30 23:36:20 by pandalaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Response.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>
#include <cstdlib>

Response::Response()
{

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

void	Response::setFile(std::string filePath)
{
	_contentType = extensionType(filePath);
	_filePath = filePath;
	FILE *	file = std::fopen(filePath.c_str(), "rb");
	if (!file)
	{
		std::cerr << "Error: Response: set: could not open file.";
		setStatusCode(500);
	}
	std::fseek(file, 0, SEEK_END);
	_fileSize = std::ftell(file);
	std::fclose(file);
}

void	Response::buildResponse()
{
	std::stringstream	headerStream;

	headerStream << "HTTP/1.1 " << _statusCode << " " << _statusMessage << "\n";
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
		headerStream << "\r\n" << "Content-Length: " << _fileSize << "\r\n";
	}
	_responseHeader = headerStream.str();
}

int	Response::sendResponse(int socketfd)
{
	int	bytesSent = 0;

	buildResponse();
	// Open file and send contents through socket
	std::ifstream	file(_filePath.c_str(), std::ios::binary);
	if (!file)
	{
		std::cerr << "Error: Response: send: could not open file.";
		return (-1);
	}
	char	buffer[1024];
	while (file.read(buffer, sizeof(buffer)))
	{
		if (bytesSent += send(socketfd, buffer, file.gcount(), 0) == -1)
		{
			std::cerr << "Error: Response: send: could not send data.";
			return (-1);
		}
	}
	if (!file.eof())
	{
		std::cerr << "Error: Response: send: could not read entire file.";
		return (-1);
	}
	file.close();
	return (bytesSent);
}
