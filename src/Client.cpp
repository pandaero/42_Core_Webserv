#include "../include/Client.hpp"

Client::Client()
{
	fd = -42;
	statusCode = 0;
	filePosition = 0;
	bytesWritten = 0;
	contentLength = 0;
	state = recv_head;
	dirListing = false;
	append = false;
}
		
void Client::parseRequest()
{
	// parse request line
	method = splitEraseStr(buffer, " ");
	path = splitEraseStr(buffer, " ");
	httpProtocol = splitEraseStr(buffer, "\r\n");
	
	// check for CGI query string
	size_t questionMarkPos = path.find("?");
	if (questionMarkPos != std::string::npos)
	{
		queryString = path.substr(questionMarkPos + 1);
		path = path.substr(0, questionMarkPos);
	}

	// parse headers and populate specific headers for easy access
	headers = parseStrMap(buffer, ":", "\r\n", "\r\n");
	if (headers.find("host") != headers.end())
		host = headers["host"].substr(0, headers["host"].find_first_of(':'));
	if (headers.find("content-length") != headers.end())
		contentLength = atoi(headers["content-length"].c_str());
	if (headers.find("content-type") != headers.end())
		contentType = headers["content-type"];
	
	// manage cookies
	if (headers.find("cookie") != headers.end())
		cookies = parseStrMap(headers["cookie"], "=", ";", "Please parse me to the end!");
	if (cookies.find(SESSIONID) != cookies.end())
	{
		sessionId = cookies[SESSIONID];
		std::string logPath = "./site/logs/" + sessionId;
		std::ofstream logFile(logPath, std::ios::app);
		if (logFile.is_open())
		{
			logFile << 
		}
		else
			std::cerr << "Could not open log file for session tracking." << std::endl;



	std::cout << "cookies:" << std::endl;
	if (!cookies.empty())
	{
		for (strMap_it it = cookies.begin(); it != cookies.end(); ++it)
		{
			std::cout 	<< "key:'" << it->first << "'. "
						<< "value:'" << it->second << "'.\n"; 
		}
		std::cout << std::endl;
	}

	// parse URL for easy access
	if (path.find("/") == std::string::npos)
		throw std::runtime_error("invalid URL in request.");
	directory = path.substr(0, path.find_last_of("/") + 1);
	filename = path.substr(path.find_last_of("/") + 1);
	if (method != POST) // we don't process bodies of GET or DELETE requests
		state = handleRequest;
	else if (contentLength <= buffer.size()) // body is already complete in this recv (header content has already been deleted from buffer)
		state = handleRequest;
	else
		state = recv_body;
}

void Client::whoIsI()
{
	std::cout << "Client on fd " << fd << std::endl;
	std::cout << "method:'" << method << "'" << std::endl;
	std::cout << "path:'" << path << "'" << std::endl;
	std::cout << "dir:'" << directory << "'" << std::endl;
	std::cout << "file:'" << filename << "'" << std::endl;
	std::cout << "content-type:'" << contentType << "'" << std::endl;
	std::cout << "standardfile:'" << standardFile << "'" << std::endl;
	std::cout << "dirlisting: " << (dirListing ? "yes" : "no") << std::endl;
}
