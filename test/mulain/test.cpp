#include <string>
#include <iostream>

/*
c++ -Wall -Werror -Wextra -std=c++98 test.cpp && ./a.out
*/

int main()
{
	time_t rawtime;
	const char* timeformat = "%G-%m-%d %H:%M:%S";
	char timeoutput[69];

	time(&rawtime);
	tm* timeinfo = localtime(&rawtime);
	strftime(timeoutput, 420, timeformat, timeinfo);
	std::cout << timeoutput << std::endl;
	std::string test = timeoutput;
	std::cout << "test size: " << test.size();
	
	//return timeoutput;
}
