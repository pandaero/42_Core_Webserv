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

Request::Request(std::string requestData)
{
	_contentLength = -1;
	_method = splitEraseStr(requestData, " ");
	_path = splitEraseStr(requestData, " ");
	_protocol = splitEraseStr(requestData, "\r\n");
	_headers = createHeaderMap(requestData, ":", "\r\n", "\r\n");
	if (headerValue("content-length") != "NOT FOUND")
		_contentLength = atoi(headerValue("content-length").c_str());
}

std::string	Request::headerValue(std::string header)
{
	std::string	query = strToLower(header);
	if (_headers.find(header) != _headers.end())
		return (_headers.find(header)->second);
	return ("NOT FOUND");
}

std::string	Request::getFile()
{
	return (_path.substr(_path.find_last_of('/'), _path.find(*(_path.end() - 1))));
}
