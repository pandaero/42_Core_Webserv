#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "webserv.hpp"

class	Client
{
	public:
		Client();

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
		size_t			contentLength;
		std::string		contentType;
		strMap			cookies;
		strMap			headers;
		
		// status vars
		state_enum		state;
		bool			append;
	
	private:
		// this "class" is a glorified struct. It only gets instantiated as a private member
		// of the Server class. That's why it doesn't have traditional privacy.
};

#endif
