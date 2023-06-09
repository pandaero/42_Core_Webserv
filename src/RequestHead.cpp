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

#include "../include/RequestHead.hpp"

RequestHead::RequestHead()
{
	_contentLength = -1;
}

RequestHead::RequestHead(std::string requestData)
{
	//DEBUG
	std::cout << "Request getting made" << std::endl;
	_contentLength = -1;
	_method = splitEraseStr(requestData, " ");
	_path = splitEraseStr(requestData, " ");
	_protocol = splitEraseStr(requestData, "\r\n");
	_headers = createHeaderMap(requestData, ":", "\r\n", "\r\n");
	if (headerValue("content-length") != "NOT FOUND")
		_contentLength = std::atoi(headerValue("content-length").c_str());
	std::cout << "Request constructed" << std::endl;
}

std::string	RequestHead::headerValue(std::string header)
{
	std::string	query = strToLower(header);
	if (_headers.find(header) != _headers.end())
		return (_headers.find(header)->second);
	return ("NOT FOUND");
}

std::string	RequestHead::getFilename() const
{
	return (_path.substr(_path.find_last_of('/'), _path.find(*(_path.end() - 1))));
}

std::string RequestHead::getMethod() const
{
	return _method;
}

std::string RequestHead::getProtocol() const
{
	return _protocol;
}

std::string	RequestHead::getPath() const
{
	return (_path);
}

int	RequestHead::getContentLength() const
{
	return (_contentLength);
}

strMap RequestHead::createHeaderMap(std::string& input, std::string endOfKey, std::string endOfValue, std::string endOfMap)
{
	strMap 		stringMap;
	std::string key, value;

	while (!input.empty())
	{
		if (input.find(endOfMap) == 0)
		{
			input = input.substr(endOfMap.size());
			return stringMap;
		}
		key = splitEraseStr(input, endOfKey);
		value = splitEraseStr(input, endOfValue);
		stringMap.insert(std::make_pair(strToLower(key), value));
	}
	return stringMap;
}
