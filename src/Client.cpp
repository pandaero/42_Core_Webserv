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
	setCookie = false;
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

	parseHeaders();
	handleCookieSession();
	
	// parse URL for easy access
	directory = path.substr(0, path.find_last_of("/") + 1);
	filename = path.substr(path.find_last_of("/") + 1);
	
	// check for body
	if (method != POST) // we don't process bodies of GET or DELETE requests
		state = handleRequest;
	else if (contentLength <= buffer.size()) // body is already complete in this recv (header content has already been deleted from buffer)
		state = handleRequest;
	else
		state = recv_body;
}

void Client::parseHeaders()
{
	headers = parseStrMap(buffer, ":", "\r\n", "\r\n");
	if (headers.find("host") != headers.end())
		host = headers["host"].substr(0, headers["host"].find_first_of(':'));
	if (headers.find("content-length") != headers.end())
		contentLength = atoi(headers["content-length"].c_str());
	if (headers.find("content-type") != headers.end())
		contentType = headers["content-type"];
}

void Client::handleCookieSession()
{
	if (headers.find("cookie") != headers.end())
		cookies = parseStrMap(headers["cookie"], "=", ";", "Please parse me to the end!");
	
	// take existing session-cookie or create new id and later transmit new session-cookie
	if (cookies.find(SESSIONID) != cookies.end())
		sessionId = cookies[SESSIONID];
	else
	{
		sessionId = generateSessionId();
		setCookie = true;
	}
	
	// write data of interest to the log
	std::string logPath = "system/logs/" + sessionId + ".log";
	std::ofstream logFile(logPath.c_str(), std::ios::app);
	if (logFile.is_open())
		logFile << currentTime() << " " << method << " " << path << "\n";
	else
		std::cerr << "Could not open log file for session tracking." << std::endl;
}

void Client::whoIsI()
{
	std::cout << "Client on fd " << fd << "\n";
	std::cout << "method:'" << method << "'" << "\n";
	std::cout << "path:'" << path << "'" << "\n";
	std::cout << "dir:'" << directory << "'" << "\n";
	std::cout << "file:'" << filename << "'" << "\n";
	std::cout << "content-type:'" << contentType << "'" << "\n";
	std::cout << "standardfile:'" << standardFile << "'" << "\n";
	std::cout << "dirlisting: " << (dirListing ? "yes" : "no") << "\n";
	std::cout << "updatedPath: " << updatedPath << "\n";
	std::cout << "updatedDirectory: " << updatedDirectory << std::endl;
}

std::string Client::generateSessionId()
{
	char sessionId[17];
	const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	size_t i = 0;
	
	srand(time(NULL));
	for (; i < sizeof(sessionId) - 1; ++i)
		sessionId[i] = charset[rand() % (sizeof(charset) - 1)];
	sessionId[i] = 0;
	return sessionId;
}