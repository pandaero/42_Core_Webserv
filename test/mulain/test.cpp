#include <string>
#include <iostream>

/*
c++ -Wall -Werror -Wextra -std=c++98 test.cpp && ./a.out
*/

int main()
{
	std::string queryString;
	std::string path = "knudel/shmang/index.html?hizzle=shmizzle";
	
	size_t questionMarkPos =  path.find("?");
	if (questionMarkPos != std::string::npos)
	{
		queryString = path.substr(questionMarkPos + 1);
		path = path.substr(0, questionMarkPos);
	}
	std::cout << "path:'" << path << "'" << std::endl;
	std::cout << "queryString:'" << queryString << "'" << std::endl;
}
