#include "../include/Request.hpp"

Request::Request()
{
	_contentLength = -1;
}

Request::Request(std::string requestData)
{
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

std::string Request::headerValue(std::string header)
{
	std::string	query = strToLower(header);
	if (_headers.find(query) != _headers.end())
		return (_headers.find(query)->second);
	return (NOTFOUND);
}

const std::string& Request::method() const
{
	return _method;
}

const std::string& Request::httpProtocol() const
{
	return _protocol;
}

const std::string& Request::path() const
{
	return _path;
}

const int& Request::contentLength() const
{
	return _contentLength;
}

const std::string& Request::contentType() const
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
