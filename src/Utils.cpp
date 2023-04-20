#include "../include/Utils.hpp"

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
