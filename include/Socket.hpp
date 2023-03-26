/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pandalaf <pandalaf@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 23:20:06 by pandalaf          #+#    #+#             */
/*   Updated: 2023/03/26 22:38:54 by pandalaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

# include "webserv.hpp"

# include <vector>
# include <exception>
# include <poll.h>
# include <pthread.h>

class	Socket;

void	startPolling(std::vector<Socket> &);
void	stopPolling();

class	Socket
{
	public:
		Socket();
		Socket(const Socket &);
		~Socket();
		Socket &	operator=(const Socket &);

		void	setHandler(void (&)(int));

		int		getPort() const;
		int		getSocketfd() const;
		void	(*getHandler() const)(int);

		void	bind(int);

	private:
		Socket(int);
		int				_port;
		void 			(*_handler)(int);
		const int		_socketfd;

	class	socketCreationFailureException: public std::exception
	{
		const char *	what() const throw();
	};
	class	bindingFailureException: public std::exception
	{
		const char *	what() const throw();
	};
};
#endif
