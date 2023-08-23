#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "webserv.hpp"

/*
This class only gets instantiated as a private member of the Server class.
That's why it doesn't have traditional encapsulation.
This would be the perfect place to use the friend keyword and make everything
private, which is however not allowed.
*/
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
		std::string		sessionId;
		
		// status vars
		state_enum		state;
		bool			append;
		bool			setCookie;
	
	private:
		void			parseHeaders();
		void			handleCookieSession();
		std::string		generateSessionId();

};

#endif
