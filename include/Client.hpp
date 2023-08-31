#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "webserv.hpp"

class	Client
{
	public:
		// Client.cpp
		Client(const ServerConfig&, pollfd&, sockaddr_in);

		void	incomingData();
		void	appendToBuf(char*, int);
		int		getFd();

		
	
	private:
		// Client.cpp
		void	sendStatusPage(int);
		void	sendFile_200(std::string);
		void	sendEmptyStatus(int);

		// _handlers.cpp
		void	handleGet();
		void	handleDelete();

		
		// _requestHead.cpp
		void requestHead();
		void parseRequestLine();
		void parseRequestHeaders();
		void handleSession();
		void selectServerConfig();
		void updateVars();
		void requestHeadError();


		
		// _utils.cpp
		std::string	ifDirAppendSlash(const std::string&);
		std::string	prependRoot(const std::string&);
		bool 		dirListing(const std::string&);
		void 		whoIsI();
		void		generateSessionLogPage(std::string);
		void		generateStatusPage(int);
		void		generateDirListingPage(const std::string&);

		void		isCgiRequest();




		const ServerConfig&	_config;
		const ServerConfig*	_activeConfig;
		pollfd&				_pollStruct;


		int				_fd; // delete?
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
		std::string			_cgiExecPath;
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
