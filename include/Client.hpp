#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "webserv.hpp"

class	Client
{
	public:
		Client();
		Client(int);
		~Client();

		void			parseRequest();
		
		int				fd;
		std::string		buffer;
		int				statusCode;
		std::string		directory;
		std::string		filename;
		std::string		sendPath;
		std::streampos	filePosition;
		size_t			bytesWritten;

		// request data
		std::string		httpProtocol;
		std::string		method;
		std::string		path;
		int				contentLength;
		std::string		contentType;
		strMap			headers;
		
		// status bools
		bool			errorPending;
		bool			requestHeadComplete;
		bool			requestBodyComplete;
		bool			responseFileSelected;
		bool			responseHeadSent;
		bool			responseBodySent;
		bool			append;
	
	private:
		void			initDefaults();
		strMap			createHeaderMap(std::string&, std::string, std::string, std::string);
};

#endif
