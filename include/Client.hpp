#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "webserv.hpp"

class	Client
{
	public:
		Client(int);
		~Client();

		void			buildRequest();
		std::string		httpProtocol();
		std::string 	method();
		std::string 	path();
		int				contentLength();
		std::string 	contentType();
	
		std::string		sendPath;
		std::streampos	filePosition;
		size_t			bytesWritten;
		int				statusCode;

		int				fd;
		std::string		buffer;
		std::string		directory;
		std::string		filename;

		bool			errorPending;
		bool			requestHeadComplete;
		bool			requestBodyComplete;
		bool			responseFileSelected;
		bool			responseHeadSent;
		bool			responseBodySent;
	
	private:
		Request*		_request;
};

#endif
