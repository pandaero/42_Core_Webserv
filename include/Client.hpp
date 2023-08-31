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
		Client(const ServerConfig&);

		void whoIsI();
		void sendStatusPage(int);
		void incomingData();
		
	
	private:
		bool requestHead();
		void parseRequestLine();
		void parseRequestHeaders();
		void selectServerConfig();

		void handleSession();
		void generateSessionLogPage(std::string);
		bool requestError();


		
		// utils
		std::string ifDirAppendSlash(const std::string&);
		std::string prependRoot(const std::string&);


		const ServerConfig&	_config;
		const ServerConfig*	_activeConfig;


		int				_fd;
		sockaddr_in		_address;
		
		std::string		_buffer;
		
		int				_statusCode;
		std::string		_sendPath;
		bool			_dirListing;
		std::string		_standardFile;
		std::streampos	_filePosition;
		size_t			_bytesWritten;
		std::string		_sessionId;

		// request data
		std::string		_method;
		std::string		_URL;
		std::string		_httpProtocol;
		std::string		_queryString;
		
		// request data ez access
		std::string		_filename;
		std::string		_directory;
		std::string		_updatedURL;
		std::string		_updatedDirectory;
		std::string		_host;
		size_t			_contentLength;
		std::string		_contentType;
		strMap			_cookies;
		strMap			_headers;
		
		// control vars
		state_enum		_state;
		bool			_append;
		bool			_setCookie;
		bool			_cgiRequest;
		time_t			_childBirth;

		// CGI helpers
		strVec				_envVec;
		std::vector<char*>	_env;
		strVec				_argvVec;
		std::vector<char*>	_argv;
		pid_t				_cgiPid;
		int 				_parentToChild[2];
		int 				_childToParent[2];
		bool				_childLaunched;
};

#endif
