#include "../include/Response.hpp"

Response::Response(){}

Response::~Response(){}

Response::Response(int code, strVec serverNames)
{
	(void)serverNames;
	std::string			httpMsg(getHttpMsg(code));
	std::stringstream	ss_body, ss_header;

	ss_body << "<!DOCTYPE html>";
	ss_body << "<html>\n";
	ss_body << "<head>\n";
	ss_body << "<title>webserv - " << code << ": " << httpMsg << "</title>\n";
	ss_body << "<style>\n";
	ss_body << "body {background-color: black; color: white; font-family: Arial, sans-serif; margin: 0; padding: 5% 0 0 0; text-align: center;}\n";
	ss_body << "h1 {font-size: 42px;}\n";
	ss_body << "p {font-size: 16px; line-height: 1.5;}\n";
	ss_body << "</style>\n";
	ss_body << "</head>\n";
	ss_body << "<body>\n";
	ss_body << "<h1>" << code << ": " << httpMsg << "</h1>\n";
	ss_body << "<img style=\"margin-left: auto;\" src=\"https://http.cat/" << code << "\" alt=\"" << httpMsg << "\">\n";
	ss_body << "</body>\n";
	ss_body << "</html>\n";
	
	ss_header << HTTPVERSION << ' ' << code << ' ' << httpMsg << "\r\n";
	ss_header << "Server: " << SERVERVERSION << "\r\n";
	ss_header << "content-type: text/html; charset=utf-8" << "\r\n";
	ss_header << "content-length: " << ss_body.str().size() << "\r\n";
	ss_header << "\r\n";

	_statusPage = ss_header.str() + ss_body.str();
	_bodySize = _statusPage.size();
}

std::string Response::getFilePath()
{
	return _filePath;
}
