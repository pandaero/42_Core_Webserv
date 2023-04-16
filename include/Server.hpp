/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 19:17:18 by pandalaf          #+#    #+#             */
/*   Updated: 2023/04/16 14:31:14 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "webserv.hpp"
# include "Client.hpp"
# include "Response.hpp"
# include "Utils.hpp"

# include <string>
# include <vector>
# include <iostream>
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
#define E_CMAXBODYINPUT	"Invalid characters in client max body size input. Only numerical allowed."
#define E_CMAXBODYVAL	"Invalid client max body size."
#define E_MAXCONNINPUT	"Invalid characters in max connections input. Only numerical allowed."
#define E_MAXCONNVAL	"Invalid size of max connections."
#define E_BACKLOGINPUT	"Invalid characters in backlog input. Only numerical allowed."
#define E_BACKLOGVAL	"Invalid size of back log."

// Constraints
#define CLIENTMAXBODYVAL	1e6
#define MAXCONNECTIONSVAL	1e4
#define BACKLOGVAL			1000

class	Server
{
	public:
		Server();
		Server(std::vector<std::string>);
		~Server();
		
		void	poll();
		void	handleConnections();
		void	whoIsI();

		// SETTERS
		void setName(std::string);
		void setHost(std::string);
		void setPort(std::string);
		
		void setGet(bool);
		void setPost(bool);
		void setDelete(bool);
		void setDirListing(bool);
		
		void setRoot(std::string);
		void setDir(std::string);
		void setUploadDir(std::string);
		void setCgiDir(std::string);
		void setErrorPage(std::string);

		void setClientMaxBody(std::string);
		void setMaxConnections(std::string);
		void setBacklog(std::string);

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
