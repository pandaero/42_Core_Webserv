#include <string>
#include <iostream>

/*
c++ -Wall -Werror -Wextra -std=c++98 test.cpp && ./a.out
*/

int main()
{
	time_t		rawtime;
	tm*			timeinfo;
	const char*	timeformat = "%G-%m-%dT%H:%M:%S";
	char		timeoutput[69];

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	//strftime(timeoutput, 420, timeformat, timeinfo);
	strftime(timeoutput, 420, timeformat, timeinfo);
	std::string	output = timeoutput;
	
	std::cout << timeoutput << std::endl;
}