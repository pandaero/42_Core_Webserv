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

void Client::whoIsI()
{
	std::stringstream title;
	title << "----- Client on fd " << fd << " with session id " << sessionId << " -----";
	std::string separator(title.str().size(), '-');
	
	std::cout	<< "\n" << title.str() << "\n"
				<< "host: " << host << "\n"
				<< "method: " << method << "\n"
				<< "URL: " << URL << "\n"
				<< "dir: " << directory << "\n"
				<< "file: " << filename << "\n"
				<< "content-length: " << contentLength << "\n"
				<< "content-type: " << contentType << "\n"
				<< "standardfile: " << standardFile << "\n"
				<< "dirlisting: " << (dirListing ? "yes" : "no") << "\n"
				<< "updatedURL: " << updatedURL << "\n"
				<< "updatedDirectory: " << updatedDirectory << "\n"
				<< separator << "\n" << std::endl;
}
