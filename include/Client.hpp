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

		void			whoIsI();
		
		int				fd;
		std::string		buffer;
		std::string		bufferCopy;
		int				statusCode;
		sockaddr_in		address;
		
		std::string		sendPath;
		bool			dirListing;
		std::string		standardFile;
		std::streampos	filePosition;
		size_t			bytesWritten;
		std::string		sessionId;

		// request data
		std::string		method;
		std::string		URL;
		std::string		httpProtocol;
		std::string		queryString;
		
		// request data ez access
		std::string		filename;
		std::string		directory;
		std::string		updatedURL;
		std::string		updatedDirectory;
		std::string		host;
		size_t			contentLength;
		std::string		contentType;
		strMap			cookies;
		strMap			headers;
		
		// control vars
		state_enum		state;
		bool			append;
		bool			setCookie;
		bool			cgiRequest;

		// CGI helpers
		strVec				envVec;
		std::vector<char*>	env;
		strVec				argvVec;
		std::vector<char*>	argv;
		pid_t				cgiPid;
		int 				parentToChild[2];
		int 				childToParent[2];
		bool				childLaunched;


		std::string			path_CGItempFile;
	
	private:
		// nothing, cause this is a class only in name, it's more of a struct.
		// It only ever gets instantiated as a private member of the Server class.
		// In real life, it would have everything private incl. constructor and have Server as friend class.
};

#endif
