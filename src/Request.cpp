/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pandalaf <pandalaf@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 22:12:48 by pandalaf          #+#    #+#             */
/*   Updated: 2023/05/03 19:34:3959 by pandalaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Request.hpp"

Request::Request()
{
	_contentLength = -1;
}

Request::Request(std::string requestData)
{
	//DEBUG
	std::cout << "Request getting made" << std::endl;
	_contentLength = -1;
	_method = splitEraseChars(requestData, " ");
	_path = splitEraseChars(requestData, " ");
	_protocol = splitEraseStr(requestData, "\r\n");
	_headers = createHeaderMap(requestData, ":", "\r\n", "\r\n");
	if (headerValue("content-length") != "NOT FOUND")
		_contentLength = std::atoi(headerValue("content-length").c_str());
	std::cout << "Request constructed" << std::endl;
}

std::string	Request::headerValue(std::string header)
{
	std::string	query = strToLower(header);
	if (_headers.find(header) != _headers.end())
		return (_headers.find(header)->second);
	return ("NOT FOUND");
}

std::string	Request::getFile() const
{
	return (_path.substr(_path.find_last_of('/'), _path.find(*(_path.end() - 1))));
}

std::string	Request::getPath() const
{
	return (_path);
}

int	Request::getContentLength() const
{
	return (_contentLength);
}
