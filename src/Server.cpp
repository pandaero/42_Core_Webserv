
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
	std::map<std::string, std::string> stringMap;
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

void Server::printRequest()
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
	requestString.erase(0, requestString.find_first_not_of(" \t\v"));

	// parse request line
	_request.Method = splitEraseStr(requestString, " ");	
	_request.URI = splitEraseStr(requestString, " ");
	_request.HTTPversion = splitEraseStr(requestString, "\r\n");

	// parse headers
	_request.Headers = splitEraseStrMap(requestString, ": ", "\n", '\n');

	// parse body (which is just what remains of the input minus the precedint newline)
	if (requestString[0] == '\n')
		requestString.erase(0, 1);
	_request.Body = requestString;
}

// GETTERS

HTTPrequest Server::getRequest() const
{
	return _request;
}

/*
GET /index.html HTTP/1.1
Host: www.example.com
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:68.0) Gecko/20100101 Firefox/68.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,;q=0.8
Accept-Language: en-US,en;q=0.5
Connection: keep-alive

[
	{
	"name": "John Doe",
	"email": "johndoe@example.com",
	"phone": "+1 555-1234"
},
{
	"name": "Jane Smith",
	"email": "janesmith@example.com",
	"phone": "+1 555-5678"
}
]

*/
