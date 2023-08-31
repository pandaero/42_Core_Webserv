#ifndef SERVER_HPP
# define SERVER_HPP

# include "webserv.hpp"

class	Server
{
	public:
		Server(const ServerConfig &);
		~Server();
		
		int	 fd();
		void whoIsI();
		void startListening(std::vector<pollfd>&);
		void acceptConnections();
		void handleConnections();

	private:
		// main handlers
		bool hangUp();
		bool receive();
		void closeClient(const char*);
		
		
		// receive, parse, errorcheck request
		bool requestHead();
		void parseRequestLine();
		void parseRequestHeaders();
		bool requestError();
		void updateClientVars();
		
		// request handlers
		void selectHostConfig();
		void handleSession();
		void handleGet();
		void handlePost();
		void handleDelete();

		// CGI 
		bool cgiRequest();
		void cgiChildGET();
		void cgiPost_launchChild();
		void cgiSendError(const char*);
		void buildCGIvars();
		bool childFinished();
		void doTheCGI();

		void			closePipes(pid_t, int[2]);

		
		// send functions		
		bool sendData();
		bool responseHead();
		void sendResponseBody();
		void sendResponseBody_CGI();
		void recvResponseBody_CGI();

		// send handlers
		void sendStatusPage(int);
		void sendFile_200(std::string);
		void sendEmptyStatus(int);
		
		void generateStatusPage(int);
		void generateSessionLogPage();
		void generateDirListingPage(const std::string&);

		// utils
		std::string		buildResponseHead();
		std::string		buildCookie(const std::string&, const std::string&, int, const std::string&);

		void			applyHostConfig(const ServerConfig&);
		std::string 	prependRoot(const std::string&);
		std::string		ifDirAppendSlash(const std::string&);

		// poll struct management
		pollfd*			getPollStruct(int);
		void			addPollStruct(int, short);
		void			removePollStruct(int);


		std::string		mimeType(const std::string&);
		bool			dirListing(const std::string&);

		int								_server_fd;
		std::vector<std::string>		_names;
		std::string						_activeServerName;
		std::vector<ServerConfig>		_configs;
		std::string						_root;
		std::string						_standardFile;
		strLocMap						_locations;
		intStrMap						_statusPagePaths;
		size_t							_clientMaxBody;
		size_t							_maxConnections;
		bool							_defaultDirListing;
		strMap							_cgiPaths;
		std::string						_cgiExecPath;
		strMap*							_mimeTypes;
		sockaddr_in						_serverAddress;

		pollfd*							_pollStruct;
		size_t							_index;
		clientVec_it					_clientIt;
		std::vector<Client>				_clients;
		std::vector<pollfd>*			_pollVector;

		int								_bytesReceived;
};

#endif
