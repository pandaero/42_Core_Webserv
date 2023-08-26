#ifndef SERVER_HPP
# define SERVER_HPP

# include "webserv.hpp"

class	Server
{
	public:
		Server(const ServerConfig &);
		~Server();
		
		int				fd();
		void			whoIsI();
		void			startListening(std::vector<pollfd>&);
		void			acceptConnections();
		void			handleConnections();

	private:
		// main handlers
		bool			hangUp();
		bool			receive();
		
		bool			requestHead();
		void			parseRequestLine();
		void			parseRequestHeaders();
		void			handleSession();
		bool			requestError();
		void			updateClientVars();
		void			selectHostConfig();


	
	
	
	
		void			handleGet();
		void			handlePost();
		void			handleDelete();
		
		bool			cgiRequest();
		void			handleCGI();
		void			buildCGIenv();
		
		void			doTheCGI();
		
		
		
		bool			sendData();
		bool			responseHead();
		void			sendResponseBody();
		
		void			sendStatusPage(int);
		void			sendFile200(std::string);
		void			sendEmptyStatus(int);
		void			closeClient(const char*);
		
		std::string		generateResponseHead();
		void			generateStatusPage(int);
		void			generateSessionLogPage();
		void			generateDirListingPage(const std::string&);


		// cookie
		std::string		makeCookie(const std::string&, const std::string&, int, const std::string&);


		// utils
		void			applyHostConfig(const ServerConfig&);
		std::string 	prependRoot(const std::string&);
		std::string		ifDirappendSlash(const std::string&);

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
		intStrMap						_errorPagesPaths;
		size_t							_clientMaxBody;
		size_t							_maxConns;
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
