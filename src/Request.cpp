/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pandalaf <pandalaf@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 22:12:48 by pandalaf          #+#    #+#             */
/*   Updated: 2023/05/02 00:02:03 by pandalaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Request.hpp"

Request::Request(std::string requestData)
{
	std::vector<std::string> requestLines;
	requestLines.reserve(100);
	requestLines = splitString(requestData, "\r\n");
	for (size_t i = 0; i < requestLines.size(); ++i)
	{
		std::pair<std::string, std::string>	headerPair;
		std::vector<std::string>	tempLine = splitString(requestLines[i], ": ");
		headerPair.first = tempLine[0];
		headerPair.second = tempLine[1];
		_headers.push_back(headerPair);
	}
}

std::string	Request::headerValue(std::string header)
{
	for(size_t i = 0; i < _headers.size(); ++i)
	{
		if (header == _headers[i].first)
			return (_headers[i].second);
	}
	return ("NOT FOUND");
}

std::string	Request::path()
{
	std::string	message = headerValue("Message");
	int	pathStart = message.find('/');
	int	httpStart = message.find("HTTP/1.1");
	return (message.substr(pathStart, httpStart));
}
