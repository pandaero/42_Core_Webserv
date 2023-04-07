/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pandalaf <pandalaf@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 19:17:18 by pandalaf          #+#    #+#             */
/*   Updated: 2023/04/07 20:58:43 by pandalaf         ###   ########.fr       */
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
		Server(std::vector<std::string>);
		~Server();
		
		void	poll();
		void	handleConnections();

	private:
		std::string			_name;
		std::string			_root;
		int					_maxConns;
		int					_numConns;
		pollfd *		_pollStructs;
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
