#include "../include/Client.hpp"

void Client::handleGet()
{
	if (_cgiRequest)
	{
		cgiChildGET();
		return;
	}
	if (isDirectory(_updatedURL))
	{
		if (resourceExists(_updatedURL + _standardFile))
			sendFile_200(_updatedURL + _standardFile);
		else if (_dirListing)
		{
			generateDirListingPage(_updatedURL);
			sendFile_200(SYS_DIRLISTPAGE);
		}
		else
			sendStatusPage(404);
	}
	else
	{
		if (resourceExists(_updatedURL))
			sendFile_200(_updatedURL);
		else
			sendStatusPage(404);
	}
}

void Client::handleDelete()
{
	if (_state > handleRequest)
		return;
	if (isDirectory(_updatedURL)) // deleting directories not allowed
		sendStatusPage(405);
	else if (!resourceExists(_updatedURL))
		sendStatusPage(404);
	else if (remove(_updatedURL.c_str()) == 0)
		sendEmptyStatus(204);
	else
		sendStatusPage(500);
}
