#include "../include/webserv.hpp"

bool isAlnumStr(const std::string& input)
{
	for (std::string::const_iterator it = input.begin(); it != input.end(); it++)
		if (!isalnum(*it))
			return false;
	return true;
}

std::string trim(std::string& input)
{
	size_t	start = input.find_first_not_of(WHITESPACE);
	size_t	end = input.find_last_not_of(WHITESPACE);
	if (start == std::string::npos)
		input = "";
	else
		input = input.substr(start, end - start + 1);
	return input;
}

std::string splitEraseStr(std::string& input, const std::string& targetString)
{
	std::string	element;
	size_t 		len;

	len = input.find(targetString);
	if (len == std::string::npos)
	{
		element = input;
		input.erase();
	}
	else
	{
		element = input.substr(0, len);
		input.erase(0, len + targetString.size());
	}
	trim(element);
	return element;
}

std::string splitEraseTrimChars(std::string& input, const std::string& targetChars)
{
	std::string element;
	size_t 		len;

	len = input.find_first_of(targetChars);
	if (len == std::string::npos)
	{
		element = input;
		input.erase();
	}
	else
	{
		element = input.substr(0, len);
		input.erase(0, len);
		trim(input);
	}
	trim(element);
	return element;
}

strVec splitEraseStrVec(std::string& input, const std::string& targetChars, const std::string& endOfParsing)
{
	strVec 		stringVector;
	std::string	parseRegion, element;
	
	parseRegion = splitEraseTrimChars(input, endOfParsing);
	input.erase(0, endOfParsing.size());
	while (!parseRegion.empty())
	{
		element = splitEraseTrimChars(parseRegion, targetChars);
		stringVector.push_back(element);
	}
	return stringVector;
}

contentType	extensionType(const std::string & filePath)
{
	size_t	pos;

	if ((pos = filePath.find_last_of('.')) == std::string::npos)
		return (OCTETSTREAM);
	if (filePath.find(".txt", pos) != std::string::npos)
		return (PLAINTEXT);
	else if (filePath.find(".css", pos) != std::string::npos)
		return (CSS);
	else if (filePath.find(".html", pos) != std::string::npos)
		return (HTML);
	else if (filePath.find(".zip", pos) != std::string::npos)
		return (ZIP);
	else if (filePath.find(".png", pos) != std::string::npos)
		return (PNG);
	else if (filePath.find(".jpeg", pos) != std::string::npos || filePath.find(".jpg", pos) != std::string::npos)
		return (JPEG);
	else if (filePath.find(".pdf", pos) != std::string::npos)
		return (PDF);
	else if (filePath.find(".xml", pos) != std::string::npos)
		return (XML);
	else if (filePath.find(".json", pos) != std::string::npos)
		return (JSON);
	else if (filePath.find(".avif", pos) != std::string::npos)
		return (AVIF);
	return (OCTETSTREAM);
}

off_t			fileSize(std::string filePath)
{
	struct	stat st;

	if (stat(filePath.c_str(), &st) == 0)
		return (st.st_size);
	return (-1);
}

std::vector<std::string>	splitString(std::string str, const std::string & del)
{
	std::vector<std::string>	out;
	out.reserve(100);
	if (str.find(del) == std::string::npos)
	{
		out.push_back(str);
		return (out);
	}
	size_t end = str.find(del);
	while (end != std::string::npos)
	{
		out.push_back(str.substr(0, end));
		str.erase(str.begin(), str.begin() + end + 1);
		end = str.find(del);
	}
	size_t	endPos = str.find(*(str.end() - 1));
	out.push_back(str.substr(0, endPos));
	return (out);
}

bool isSameNoCase(const std::string& str_a, const std::string& str_b)
{
	return strToLower(str_a) == strToLower(str_b);
}

std::string strToLower(std::string str)
{
	for (std::string::iterator it = str.begin(); it != str.end(); it++)
		*it = tolower(*it);
	return str;
}

std::string getInstruction(std::string& inputStr)
{
	std::string	instruction;
	size_t		len_semicolon;

	// Check whether semicolon delimits the instruction
	len_semicolon = inputStr.find(";");
	if (len_semicolon < inputStr.find("{"))
	{
		instruction = inputStr.substr(0, len_semicolon);
		inputStr.erase(0, len_semicolon + 1);
		trim(instruction);
		return instruction;
	}

	// Curly braces must now delimit the instruction
	size_t	i;
	int		bracesDepth;

	i = inputStr.find("{");
	if (i == std::string::npos || inputStr.find("}") < i)
		throw std::runtime_error(E_INVALIDENDTOKEN + inputStr + '\n');
	bracesDepth = 1;
	while (inputStr[++i] && bracesDepth > 0 && bracesDepth < 3)
	{
		if (inputStr[i] == '{')
			bracesDepth++;
		else if (inputStr[i] == '}')
			bracesDepth--;
	}
	if (bracesDepth != 0)
		throw std::runtime_error(E_INVALIDBRACE + inputStr + '\n');
	instruction = inputStr.substr(0, i);
	inputStr.erase(0, i);
	instruction.replace(instruction.find("{"), 1, " ");
	instruction.replace(instruction.find_last_of("}"), 1, " ");
	trim(instruction);
	return instruction;
}

bool resourceExists(const std::string& path)
{
	std::ifstream	resource(path.c_str());
	
	return resource.good();
}

bool isDirectory(const std::string& path)
{
	struct stat	fileStat;
	
	if (stat(path.c_str(), &fileStat) == 0)
		return S_ISDIR(fileStat.st_mode);
	return false;
}

std::string getHttpMsg(int code)
{
	switch (code)
	{
		case 100: return "Continue";
		case 101: return "Switching Protocols";
		case 102: return "Processing";
		case 103: return "Checkpoint";

		case 200: return "OK";
		case 201: return "Created";
		case 202: return "Accepted";
		case 203: return "Non-Authoritative Information";
		case 204: return "No Content";
		case 205: return "Reset Content";
		case 206: return "Partial Content";
		case 207: return "Multi-Status";
		case 208: return "Already Reported";

		case 300: return "Multiple Choices";
		case 301: return "Moved Permanently";
		case 302: return "Found";
		case 303: return "See Other";
		case 304: return "Not Modified";
		case 305: return "Use Proxy";
		case 306: return "Switch Proxy";
		case 307: return "Temporary Redirect";
		case 308: return "Permanent Redirect";

		case 400: return "Bad Request";
		case 401: return "Unauthorized";
		case 402: return "Payment Required";
		case 403: return "Forbidden";
		case 404: return "Not Found";
		case 405: return "Method Not Allowed";
		case 406: return "Not Acceptable";
		case 407: return "Proxy Authentication Required";
		case 408: return "Request Timeout";
		case 409: return "Conflict";
		case 410: return "Gone";
		case 411: return "Length Required";
		case 412: return "Precondition Failed";
		case 413: return "Payload Too Large";
		case 414: return "URI Too Long";
		case 415: return "Unsupported Media Type";
		case 416: return "Requested Range Not Satisfiable";
		case 417: return "Expectation Failed";
		case 418: return "I'm a teapot";
		case 421: return "Misdirected Request";
		case 422: return "Unprocessable Entity";
		case 423: return "Locked";
		case 424: return "Failed Dependency";
		case 426: return "Upgrade Required";
		case 428: return "Precondition Required";
		case 429: return "Too Many Request";
		case 431: return "Request Header Fields Too Large";
		case 451: return "Unavailable For Legal Reasons";

		case 500: return "Internal Server Error";
		case 501: return "Not Implemented";
		case 502: return "Bad Gateway";
		case 503: return "Service Unavailable";
		case 504: return "Gateway Timeout";
		case 505: return "HTTP Version Not Supported";
		case 506: return "Variant Also Negotiates";
		case 507: return "Insufficient Storage";
		case 508: return "Loop Detected";
		case 510: return "Not Extended";
		case 511: return "Network Authentication Required";

		default: return "Unknown Error";
	}
}
