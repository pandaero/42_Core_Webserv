
#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <map>
#include <vector>

typedef std::map<std::string, std::string>::const_iterator	StringMap_it;
typedef std::map<std::string, std::string> 					StringMap;

bool 			isAlnumString(const std::string&);
std::string		trim(std::string& input);
std::string		splitEraseStr(std::string&, std::string);
StringMap		splitEraseStrMap(std::string&, std::string, std::string, char);

#endif
