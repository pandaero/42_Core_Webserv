/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 17:05:35 by pandalaf          #+#    #+#             */
/*   Updated: 2023/05/06 19:27:42 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Response.hpp"

Response::Response()
{

}

Response::Response(int errNum, const Server & server)
{
	setStatusCode(errNum);
	setFile(server.);
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
	// DEBUG
	std::cout << "Opening: " << filePath << std::endl;
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

void	Response::build()
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
	_responseHeader = headerStream.str();
}

int	Response::send(int socketfd)
{
	int headerBytesSent = 0;
	int	fileBytesSent = 0;
	int	closingBytesSent = 0;

	build();
	// Send header
	if ((headerBytesSent += ::send(socketfd, _responseHeader.data(), _responseHeader.size(), 0)) == -1)
	{
		std::cerr << "Error: Response: send: failure to send header data.";
		return (-1);
	}
	// Open file and send contents through socket
	std::ifstream	file(_filePath.c_str(), std::ios::binary);
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
	if (!file.eof())
	{
		std::cerr << "Error: Response: send: could not read entire file.";
		return (-1);
	}
	// DEBUG
	std::cout << "Reached EOF" << std::endl;
	file.close();
	// Send termination CRLFs
	std::string	terminationSequence("\r\n\r\n");
	if ((closingBytesSent += ::send(socketfd, terminationSequence.data(), terminationSequence.size(), 0)) == -1)
	{
		std::cerr << "Error: Response: send: failure to send termination data.";
		return (-1);
	}
	return (fileBytesSent);
}
