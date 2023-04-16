/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 19:17:18 by pandalaf          #+#    #+#             */
/*   Updated: 2023/04/16 09:04:11 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "webserv.hpp"
# include "Client.hpp"
# include "Response.hpp"

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

	private:
		std::string			_name;
		in_addr_t			_host;
		uint16_t			_port;
		
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
		int					_maxConns;
		int					_numConns;
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
