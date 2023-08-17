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

	errorPending = false;
	requestHeadComplete = false;
	requestBodyComplete = false;
	responseFileSelected = false;
	responseHeadSent = false;
	responseBodySent = false;
	append = false;
}

Client::~Client()
{
	std::cout << "client destructor fd " << fd << std::endl;
}
		
void Client::parseRequest()
{
	// parse request line
	method = splitEraseStr(buffer, " ");
	path = splitEraseStr(buffer, " ");
	httpProtocol = splitEraseStr(buffer, "\r\n");
	
	// parse headers and populate specific headers for easy access
	headers = createHeaderMap(buffer, ":", "\r\n", "\r\n");
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
