#include "../include/Client.hpp"

void Client::handleGet()
{
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
	if (isDirectory(_updatedURL)) // deleting directories not allowed
		sendStatusPage(405);
	else if (!resourceExists(_updatedURL))
		sendStatusPage(404);
	else if (remove(_updatedURL.c_str()) == 0)
		sendEmptyStatus(204);
	else
		sendStatusPage(500);
}

void Client::handlePost()
{
	std::ofstream	outputFile;
	
	if (_append)
		outputFile.open(_updatedURL.c_str(), std::ios::binary | std::ios::app);
	else
	{
		outputFile.open(_updatedURL.c_str(), std::ios::binary | std::ios::trunc);
		_append = true;
	}
	if (!outputFile)
	{
		outputFile.close();
		std::cerr << E_C_OFSTREAM << std::endl;
		sendStatusPage(500);
		return;		
	}
	outputFile.write(_buffer.c_str(), _buffer.size());
	_bytesWritten += _buffer.size();
	_buffer.clear();
	outputFile.close();

	if (_bytesWritten >= _contentLength)
		sendEmptyStatus(201);
}

void Client::handleGetCGI()
{

}

void Client::handlePostCGI()
{
	
}
