#include "../include/Client.hpp"

Client::Client()
{
	initDefaults();
}

/*
Client::Client(int socketfd): Client()
-> not possible in cpp98, so have to use initDefaults()
*/
Client::Client(int socketfd)
{
	initDefaults();
	fd  = socketfd;
}

void Client::initDefaults()
{
	fd = -42;
	statusCode = 0;
	filePosition = 0;
	bytesWritten = 0;
	contentLength = -1;

	dirListing = false;
	errorPending = false;
	requestHeadComplete = false;
	requestBodyComplete = false;
	requestFinished = false;
	responseHeadSent = false;
	append = false;
}

Client::~Client()
{
	std::cout << "Client destructor on fd " << fd << std::endl;
}
		
void Client::parseRequest()
{
	// parse request line
	method = splitEraseStr(buffer, " ");
	path = splitEraseStr(buffer, " ");
	httpProtocol = splitEraseStr(buffer, "\r\n");
	
	// check for CGI query string
	size_t questionMarkPos =  path.find("?");
	if (questionMarkPos != std::string::npos)
	{
		queryString = path.substr(questionMarkPos + 1);
		path = path.substr(0, questionMarkPos);
	}

	// parse headers and populate specific headers for easy access
	headers = createHeaderMap(buffer, ":", "\r\n", "\r\n");
	if (headers.find("host") != headers.end())
		host = headers["host"].substr(0, headers["host"].find_first_of(':'));
	if (headers.find("content-length") != headers.end())
		contentLength = atoi(headers["content-length"].c_str());
	if (headers.find("content-type") != headers.end())
		contentType = headers["content-type"];

	// parse URL for easy access
	if (path.find("/") == std::string::npos)
		throw std::runtime_error("invalid URL in request.");
	directory = path.substr(0, path.find_last_of("/") + 1);
	filename = path.substr(path.find_last_of("/") + 1);
	if (contentLength <= 0 || method != POST) // we don't process bodies of GET or DELETE requests
		requestBodyComplete = true;
	requestHeadComplete = true;
}

void Client::whoIsI()
{
	std::cout << "Client on fd " << fd << std::endl;
	std::cout << "method:'" << method << "'" << std::endl;
	std::cout << "path:'" << path << "'" << std::endl;
	std::cout << "dir:'" << directory << "'" << std::endl;
	std::cout << "file:'" << filename << "'" << std::endl;
	std::cout << "standardfile:'" << standardFile << "'" << std::endl;
	std::cout << "dirlisting: " << (dirListing ? "yes" : "no") << std::endl;
}

strMap Client::createHeaderMap(std::string& input, std::string endOfKey, std::string endOfValue, std::string endOfMap)
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
