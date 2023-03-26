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
	
	std::string requestString("GETS knudeld");
	RequestHandler rHandler(requestString);

	request requesty = rHandler.getRequest();
	std::cout << requesty.method << std::endl;
}