#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "webserv.hpp"

class	Client
{
	public:
		Client();
		~Client();

		void			parseRequest();
		void			whoIsI();
		
		int				fd;
		std::string		buffer;
		int				statusCode;
		std::string		directory;
		std::string		filename;
		std::string		sendPath;
		bool			dirListing;
		std::string		standardFile;
		std::streampos	filePosition;
		size_t			bytesWritten;

		// request data
		std::string		httpProtocol;
		std::string		method;
		std::string		path;
		std::string		queryString;
		std::string		host;
		int				contentLength;
		std::string		contentType;
		strMap			headers;
		
		// status bools
		bool			errorPending;
		bool			requestHeadComplete;
		bool			requestBodyComplete;
		bool			requestFinished;
		bool			responseHeadSent;
		bool			append;
	
	private:
		strMap			createHeaderMap(std::string&, std::string, std::string, std::string);
};

#endif
