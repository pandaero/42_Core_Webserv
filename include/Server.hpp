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
		bool requestHead();
		void parseRequestLine();
		void parseRequestHeaders();
		void handleSession();
		bool requestError();
		void updateClientVars();
		void selectHostConfig();

		// request handlers
		void handleGet();
		void handlePost();
		void handleDelete();
		bool cgiRequest();
		void handleCGI();
		void cgiPost_launchChild();
		void buildCGIvars();
		
		void			doTheCGI();
		
		
		
		bool			sendData();
		bool			responseHead();
		void			sendResponseBody();
		void			sendResponseBody_CGI();
		
		void			sendStatusPage(int);
		void			sendFile200(std::string);
		void			sendCGI200();

		void			sendEmptyStatus(int);
		void			closeClient(const char*);
		
		void			generateStatusPage(int);
		void			generateSessionLogPage();
		void			generateDirListingPage(const std::string&);

		std::string		buildResponseHead();
		strVec			buildCGIenv();
		std::string		buildCookie(const std::string&, const std::string&, int, const std::string&);

		void			manageCGIpipes_child(int[2]);
		void			closePipes(pid_t, int[2]);

		// utils
		void			applyHostConfig(const ServerConfig&);
		std::string 	prependRoot(const std::string&);
		std::string		ifDirAppendSlash(const std::string&);

		pollfd*			getPollStruct(int);
		std::string		mimeType(const std::string&);
		bool			dirListing(const std::string&);

		// setters
		void			setHost(std::string);
		void			setPort(std::string);
		void			setClientMaxBody(std::string);
		void			setMaxConnections(std::string); //unused
		void			setDefaultDirListing(std::string);

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
