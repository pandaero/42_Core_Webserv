/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 19:17:18 by pandalaf          #+#    #+#             */
/*   Updated: 2023/04/19 12:04:10 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "webserv.hpp"
# include "Client.hpp"
# include "Response.hpp"
# include "Config.hpp"

# include <string>
# include <vector>
# include <iostream>
# include <sstream>
# include <exception>
# include <algorithm>
# include <cstdio>
# include <cstdlib>
# include <fcntl.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <sys/socket.h>
# include <sys/poll.h>
# include <netinet/in.h>

// Error messages
#define E_SERVERNAME	"Invalid characters in server name input. Only alphanumerical allowed."
#define E_HOSTADDRINPUT	"Invalid characters in host address input. Only numerical and dot allowed."
#define E_HOSTADDRVAL	"Invalid address value. Cannot convert to IP address."
#define E_PORTINPUT		"Invalid characters in port input. Only numerical allowed."
#define E_PORTVAL		"Invalid port number. Must be from 0 to 65535."
#define E_MAXCLIENTBODYINPUT	"Invalid characters in client max body size input. Only numerical allowed."
#define E_MAXCLIENTBODYVAL		"Invalid client max body size."
#define E_MAXCONNINPUT	"Invalid characters in max connections input. Only numerical allowed."
#define E_MAXCONNVAL	"Invalid size of max connections."
#define E_BACKLOGINPUT	"Invalid characters in backlog input. Only numerical allowed."
#define E_BACKLOGVAL	"Invalid size of back log."
#define E_ACC_READ		"Cannot read from path: "
#define E_ACC_WRITE		"Cannot write in path: "

// Constraints
#define MAX_MAXCLIENTBODY	10000
#define MAX_MAXCONNECTIONS	1000
#define MAX_BACKLOG			100

struct serverConfig; //why necessary? It's in Config.hpp

class	Server
{
	public:
		Server();
		Server(serverConfig);
		~Server();
		
		void	startListening();
		void	poll();
		void	handleConnections();
		void	whoIsI();

		// SETTERS
		void	setName(std::string);
		void	setHost(std::string);
		void	setPort(std::string);
		void	setPost(bool);
		
		void	setDelete(bool);
		void	setGet(bool);
		void	setDirListing(bool);
		
		void	setRoot(std::string);
		void	setDir(std::string);
		void	setUploadDir(std::string);
		void	setCgiDir(std::string);
		void	setErrorPage(std::string);

		void	setClientMaxBody(std::string);
		void	setMaxConnections(std::string);
		void	setBacklog(std::string);

	private:
		std::string			_name;
		
		bool				_GET;
		bool				_POST;
		bool				_DELETE;
		bool				_dirListing;
		
		std::string			_root;
		std::string			_dir;
		std::string			_uploadDir;
		std::string			_cgiDir;
		std::string			_errorPage;
		
		size_t				_clientMaxBody;
		size_t				_backlog;
		size_t				_maxConns;
		size_t				_numConns;
		pollfd *			_pollStructs;
		sockaddr_in			_serverAddress;
		std::vector<Client>	_clients;

		void	checkMethodAccess(std::string);
		void	checkReadAccess(std::string);
		void	checkWriteAccess(std::string);

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
	class	connectionLimitExceededException: public std::exception
	{
		const char *	what() const throw();
	};
	class	sendFailureException: public std::exception
	{
		const char *	what() const throw();
	};
};

#endif
