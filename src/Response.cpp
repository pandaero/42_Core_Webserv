/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pandalaf <pandalaf@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 17:05:35 by pandalaf          #+#    #+#             */
/*   Updated: 2023/04/02 12:17:36 by pandalaf         ###   ########.fr       */
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

Response::Response(const Response & other)
{
	_fullResponse = other._fullResponse;
	_statusCode = other._statusCode;
	_statusMessage = other._statusMessage;
	_webPage = other._webPage;
}

Response::~Response()
{

}

Response &	Response::operator=(const Response & other)
{
	if (this != &other)
	{
		_fullResponse = other._fullResponse;
		_statusCode = other._statusCode;
		_statusMessage = other._statusMessage;
		_webPage = other._webPage;
	}
	return (*this);
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
		default:
			break;
	}
}

void	Response::loadPage(std::string filename)
{
	std::ifstream	page(filename.c_str());

	if (page.fail())
		std::cerr << "Error: could not open webpage file." << std::endl;
	
	std::stringstream	pageStream;
	pageStream << page.rdbuf();
	_webPage = pageStream.str();
	page.close();
}

void	Response::buildResponse()
{
	std::stringstream	responseStream;

	responseStream << "HTTP/1.1 " << _statusCode << " " << _statusMessage << "\n";
	if (!_webPage.empty())
	{
		responseStream << "Content-Type: html\r\n" << "Content-Length: " << _webPage.size() << "\r\n";
		responseStream << "\r\n" << _webPage;
	}
	_fullResponse = responseStream.str();
}

const void *	Response::send_msg()
{
	return (_fullResponse.data());
}

int	Response::send_size()
{
	return (_fullResponse.size());
}
