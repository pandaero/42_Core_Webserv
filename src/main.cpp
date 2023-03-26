#include <cstdlib>
#include <RequestHandler.hpp>

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
	
	std::string requestString("PUT A B");
	RequestHandler rHandler(requestString);

	request clientRequest = rHandler.getRequest();
	std::cout << clientRequest.method << std::endl;
	std::cout << clientRequest.HTTPversion << std::endl;
	std::cout << clientRequest.URI << std::endl;
}