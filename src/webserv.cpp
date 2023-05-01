#include "../include/webserv.hpp"

bool isAlnumString(const std::string& input)
{
	for (std::string::const_iterator it = input.begin(); it != input.end(); it++)
		if (!isalnum(*it))
			return false;
	return true;
}

std::string trim(std::string& input)
{
	size_t start = input.find_first_not_of(" \t\r\n");
	size_t end = input.find_last_not_of(" \t\r\n");
	if (start == std::string::npos)
		input = "";
	else
		input = input.substr(start, end - start + 1);
	return input;
}

std::string splitEraseStr(std::string& input, std::string target)
{
	std::string element;
	size_t len;

	element = input.substr(0, len = input.find(target));
	input.erase(0, len + target.length());
	return element;
}

StringMap splitEraseStrMap(std::string& input, std::string endOfKey, std::string endOfValue, char endOfMap)
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

contentType	extensionType(const std::string & filePath)
{
	size_t	pos;

	if ((pos = filePath.find_last_of('.')) == std::string::npos)
		return (OCTETSTREAM);
	if (filePath.find(".txt", pos) != std::string::npos)
		return (PLAINTEXT);
	else if (filePath.find(".html", pos) != std::string::npos)
		return (HTML);
	else if (filePath.find(".zip", pos) != std::string::npos)
		return (ZIP);
	else if (filePath.find(".PNG", pos) != std::string::npos)
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

std::vector<std::string>	splitString(std::string str, const std::string & del)
{
	std::vector<std::string>	out;
	out.reserve(100);
	if (str.find(del) == std::string::npos)
	{
		out.push_back(str);
		return (out);
	}
	int end = str.find(del);
	while (end != -1)
	{
		out.push_back(str.substr(0, end));
		str.erase(str.begin(), str.begin() + end + 1);
		end = str.find(del);
	}
	out.push_back(str.substr(0, end));
	return (out);
}
