#include <cstdlib>
#include <Server.hpp>

/* int	main(int argc, char **argv)
{

	// PARSE CONFIG FILE

	// SET UP SERVER

	// PARSE INSTRUCTIONS

	// EXECUTE INSTRUCTIONS

	return (EXIT_SUCCESS);
}
 */

int main (void)
{
	
	std::string requestString("   	PUT /sdf.knudel HTTP:1.1\r\niku: shmag\nknudel: knang:sdfs\n\nknu: ksdfi");
	Server serverA;

	serverA.parseRequest(requestString);
	serverA.printRequest();
}
