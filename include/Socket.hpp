/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pandalaf <pandalaf@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 23:20:06 by pandalaf          #+#    #+#             */
/*   Updated: 2023/03/29 13:58:13 by pandalaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

# include "Client.hpp"
# include "webserv.hpp"

# include <map>
# include <string>
# include <vector>
# include <exception>
# include <poll.h>
# include <pthread.h>
# include <arpa/inet.h>

class	Socket
{
	public:
		Socket();
		Socket(const Socket &);
		~Socket();
		Socket &	operator=(const Socket &);

		int		getPort() const;
		int		getSocketfd() const;

		void	assignAddress(std::string);
		void	bind(int);
		void	listen();

		static void		poll();

	private:
		Socket(int);
		const int										_socketfd;
		int												_port;
		struct sockaddr_in								_serverAddress;
		static std::vector<int>							_serverSocketfds;
		static std::vector<int>							_clientSocketfds;
		static int										_numSockets;
		static int										_numPolledfds;
		static pollfd *									_pollStruct;
		static std::map<int, std::vector<Client> >	_connectionMap;

	class	socketCreationFailureException: public std::exception
	{
		const char *	what() const throw();
	};
	
	class	bindingFailureException: public std::exception
	{
		const char *	what() const throw();
	};
	
	class	listenFailureException: public std::exception
	{
		const char *	what() const throw();
	};

	class	invalidAddressException: public std::exception
	{
		const char *	what() const throw();
	};
};
#endif
