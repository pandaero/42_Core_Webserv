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
	ANNOUNCEME
	_method = splitEraseStr(requestData, " ");
	_path = splitEraseStr(requestData, " ");
	_protocol = splitEraseStr(requestData, "\r\n");
	_headers = createHeaderMap(requestData, ":", "\r\n", "\r\n");
	if (headerValue("content-length") != NOTFOUND)
		_contentLength = atoi(headerValue("content-length").c_str());
	else
		_contentLength = -1;
	if (headerValue("content-type") != NOTFOUND)
		_contentType = headerValue("content-type");
}

std::string	Request::headerValue(std::string header) const
{
	std::string	query = strToLower(header);
	if (_headers.find(query) != _headers.end())
		return (_headers.find(query)->second);
	return (NOTFOUND);
}

//prolly not needed
std::string	Request::getFilename() const
{
	return _path.substr(_path.find_last_of('/'), _path.size() - 1);
}

std::string Request::method() const
{
	return _method;
}

std::string Request::httpProtocol() const
{
	return _protocol;
}

std::string	Request::path() const
{
	return _path;
}

int	Request::contentLength() const
{
	return _contentLength;
}

std::string Request::getContentType() const
{
	return _contentType;
}

strMap Request::createHeaderMap(std::string& input, std::string endOfKey, std::string endOfValue, std::string endOfMap)
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
