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
		void			handleGet();
		void			handlePost();
		void			handleDelete();
		bool			sendData();
		bool			responseHead();
		void			sendResponseBody();
		
		std::string		buildResponseHead();
		void			updateClientPath();
		void			sendStatusPage(int);
		void			sendFile200(std::string);
		void			sendEmptyStatus(int);
		void			generateStatusPage(int);
		void			generateCookieLogPage();
		void			selectHostConfig();
		bool			requestError();
		void			closeClient(const char*);
		bool			cgiRequest();
		void			doTheCGI();

		// cookie
		std::string		makeCookie(const std::string&, const std::string&, int, const std::string&);


		// utils
		void			applyHostConfig(const ServerConfig&);
		std::string 	prependRoot(const std::string&);
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
		std::string						_cgiExtension;
		strMap*							_mimeTypes;
		sockaddr_in						_serverAddress;

		pollfd*							_pollStruct;
		size_t							_index;
		clientVec_it					_clientIt;
		std::vector<Client>				_clients;
		std::vector<pollfd>*			_pollVector;
};

#endif
