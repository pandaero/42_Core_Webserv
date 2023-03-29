
#include "../include/Server.hpp"

// Constructors and destructors
Server::Server(void)
{}

Server::~Server(void)
{}

Server::Server(const Server& src)
{
	*this = src;
}

// Operator overloads
Server& Server::operator=(const Server& src)
{
	_request = src._request;
	return *this;
}

// UTILS

std::string Server::splitEraseStr(std::string& input, std::string target)
{
	std::string element;
	size_t len;

	element = input.substr(0, len = input.find(target));
	input.erase(0, len + target.length());
	return element;
}

StringMap Server::splitEraseStrMap(std::string& input, std::string endOfKey, std::string endOfValue, char endOfMap)
{
	StringMap stringMap;
	std::string key, value;

	while (!input.empty() && input[0] != endOfMap)
	{
		key = splitEraseStr(input, endOfKey);
		value = splitEraseStr(input, endOfValue);
		stringMap.insert(std::make_pair(key, value));
	}
	return stringMap;
}

// ACTIONS

void Server::printRequest() const
{
	std::cout << "Method: " << _request.Method << "\n";
	std::cout << "URI: " << _request.URI << "\n";
	std::cout << "HTTP: " << _request.HTTPversion << std::endl;
	std::cout << "Headers:" << std::endl;
	for (StrMap_it it = _request.Headers.begin(); it != _request.Headers.end(); it++)
		std::cout << it->first << ": " << it->second << std::endl;
	std::cout << "Body: " << _request.Body << std::endl;
}

void Server::parseRequest(std::string requestString)
{
	// remove possible leading whitespace that is not syntax relevant (\r\n are relevant!)
	// maybe remove this because this shouldnt actually happen
	requestString.erase(0, requestString.find_first_not_of(" \t\v"));

	// parse request line
	_request.Method = splitEraseStr(requestString, " ");	
	_request.URI = splitEraseStr(requestString, " ");
	_request.HTTPversion = splitEraseStr(requestString, "\r\n");

	// parse headers
	_request.Headers = splitEraseStrMap(requestString, ": ", "\n", '\n');

	// parse body (which is just what remains of the input minus the preceding newline)
	if (requestString[0] == '\n')
		requestString.erase(0, 1);
	_request.Body = requestString;
}

// GETTERS

HTTPrequest Server::getRequest() const
{
	return _request;
}
