#ifndef SERVER_HPP
# define SERVER_HPP

# include "webserv.hpp"

class	Server
{
	public:
		Server(const ServerConfig &);
		~Server();
		
		void	whoIsI();
		void	startListening();
		void	poll();
		void	acceptConnections();
		void	handleConnections();
	
	private:
		void	receiveData();
		void	handleRequestHead();
		void	handleRequestBody();
		void	selectResponseContent();
		void	sendResponseHead();
		void	sendResponseBody();

		void	closeClient(const char*);
		
		std::string		mimeType(std::string);
		void			checkRequest();
		void			selectErrorPage(int);
		void			generateErrorPage(int);

		std::string		getStatusPage(int) const;
		bool			dirListing(const std::string&);

		void	setNames(std::string);
		void	setHost(std::string);
		void	setPort(std::string);
		void	setRoot(std::string);
		//void	setDir(std::string);
		void	setUploadDir(std::string);
		//void	setCgiDir(std::string);
		void	setClientMaxBody(std::string);
		void	setMaxConnections(std::string);
		void	setDefaultDirListing(std::string input);

		int								_server_fd;
		std::vector<std::string>		_names;
		strLocMap						_locations;
		intStrMap						_errorPagesPaths;
		size_t							_clientMaxBody;
		size_t							_maxConns;
		bool							_defaultDirListing;
		strMap							_cgiPaths;
		strMap*							_mimeTypes;

		sockaddr_in						_serverAddress;
		int								_clientfd;
		clientVec_it					_clientIt;
		std::vector<Client>				_clients;
		std::vector<pollfd>				_pollVector;
		size_t							_index;
		
		std::string						_root;
		std::string						_standardFileName;
		std::string						_uploadDir;
		/* void	checkMethodAccess(std::string);
		void	checkReadAccess(std::string);
		void	checkWriteAccess(std::string);
		void	checkExecAccess(std::string); */

	class	errorCodeResponse: public std::exception
	{};
	
	
	
	
	class	invalidAddressException: public std::exception
	{
		const char *	what() const throw();
	};
	class	socketCreationFailureException: public std::exception
	{
		const char *	what() const throw();
	};
	class	fileDescriptorControlFailureException: public std::exception
	{
		const char *	what() const throw();
	};
	class	bindFailureException: public std::exception
	{
		const char *	what() const throw();
	};
	class	listenFailureException: public std::exception
	{
		const char *	what() const throw();
	};
	class	pollFailureException: public std::exception
	{
		const char *	what() const throw();
	};
	class	sendFailureException: public std::exception
	{
		const char *	what() const throw();
	};
};
#endif
