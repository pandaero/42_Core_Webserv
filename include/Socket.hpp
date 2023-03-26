/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pandalaf <pandalaf@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 23:20:06 by pandalaf          #+#    #+#             */
/*   Updated: 2023/03/26 04:26:54 by pandalaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

# include "webserv.hpp"

# include <vector>
# include <pthread.h>
# include <exception>

class	Socket
{
	public:
		Socket();
		Socket(const Socket &);
		~Socket();
		Socket &	operator=(const Socket &);

		void	bind(int);
		void	listen();
		void	start(void * (&)(void *));
		void	stop();

	private:
		Socket(int);
		int								_port;
		const int						_socketfd;
		static std::vector<pthread_t>	_socketThreads;

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
};
#endif
