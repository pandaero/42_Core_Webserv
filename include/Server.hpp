/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 19:17:18 by pandalaf          #+#    #+#             */
/*   Updated: 2023/06/09 19:04:38 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "webserv.hpp"
# include "Client.hpp"
# include "Response.hpp"
# include "RequestHead.hpp"
# include "ServerConfig.hpp"

# include <string>
# include <list>
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

class ServerConfig;

class	Server
{
	public:
		friend class	Response;
		
		// DEBUG
		Server();

		Server(const ServerConfig &);
		~Server();
		
		void	startListening();
		void	poll();
		void	handleConnections();
		void	checkNewClients();
		bool	checkEvent(size_t);
		bool	closeClient(size_t, clientVec_it);
		void	handleRequestBegin(clientVec_it);
		bool	requestTopCompleteNow(clientVec_it);
		void	whoIsI();

		void	setNames(std::string);
		void	setHost(std::string);
		void	setPort(std::string);
		void	setRoot(std::string);
		void	setDir(std::string);
		void	setUploadDir(std::string);
		void	setCgiDir(std::string);
		void	setClientMaxBody(std::string);
		void	setMaxConnections(std::string);
		void	setBacklog(std::string);
		void	setGlobalDirListing(std::string input);

		std::string	getStatusPage(int) const;
		std::string	getRoot() const;
		
		void	errorHandler(int, int);

	private:
		std::vector<std::string>		_names;
		strLocMap						_locations;
		intStrMap						_errorPagesPaths;
		size_t							_clientMaxBody;
		size_t							_maxConns;
		bool							_defaultDirListing;
		strMap							_cgiPaths;

		pollfd *						_pollStructs;
		sockaddr_in						_serverAddress;
		std::vector<Client>				_clients;
		char							_recvBuffer[RECV_CHUNK_SIZE];
		size_t							_bytesReceived;
		
		// TO BE DEMOLISHED:
		strMap							_filePaths;
/* 		bool							_GET;
		bool							_POST;
		bool							_DELETE; */
		std::string						_root;
		std::string						_dir;
		std::string						_uploadDir;
		std::string						_cgiDir;
		size_t							_backlog; // kill this?
		size_t							_numConns;

		/* void	checkMethodAccess(std::string);
		void	checkReadAccess(std::string);
		void	checkWriteAccess(std::string);
		void	checkExecAccess(std::string); */

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
