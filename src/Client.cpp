#include "../include/Client.hpp"

Client::Client(int pollStructIndex)
{
	fd = -42;
	pollStructIndex = pollStructIndex;

	contentLength = -1;
	filePosition = 0;
	
	errorPending = false;
	requestHeadComplete = false;
	requestBodyComplete = false;
	responseFileSelected = false;
	responseHeadSent = false;
}

Client::~Client()
{
	std::cout << "client destructor fd " << fd << std::endl;
}

void Client::reset()
{
	contentLength = -1;
	filePosition = 0;
	
	errorPending = false;
	requestHeadComplete = false;
	requestBodyComplete = false;
	responseFileSelected = false;
	responseHeadSent = false;
}
		
void Client::parseRequest()
{
	method = splitEraseStr(buffer, " ");
	path = splitEraseStr(buffer, " ");
	httpProtocol = splitEraseStr(buffer, "\r\n");
	headers = createHeaderMap(buffer, ":", "\r\n", "\r\n");
	if (headers.find("content-length") != headers.end())
		contentLength = atoi(headers["content-length"].c_str());
	if (headers.find("content-type") != headers.end())
		contentType = headers["content-type"];
	//buffer.erase(0, buffer.find("\r\n\r\n") + 4);
	directory = path.substr(0, path.find_last_of("/") + 1);
	if (contentLength<= 0 || method != POST) // we don't process bodies of GET or DELETE requests
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
