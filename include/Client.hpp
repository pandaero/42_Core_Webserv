/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pandalaf <pandalaf@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 20:29:39 by pandalaf          #+#    #+#             */
/*   Updated: 2023/05/03 19:14:01 by pandalaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Request.hpp"

# include <sstream>
# include <exception>
# include <arpa/inet.h>
# include <sys/socket.h>

class	Client
{
	public:
		friend class	Server;

		Client(int);

		int		getSocketfd();

	private:
		int					_clientSocketfd;
		socklen_t			_clientSockLen;
		sockaddr_in			_clientAddress;
		std::stringstream	_recvBuffer;
		Request				_activeRequest;
		bool				_gotRequest;

	class	connectionDeniedException: public std::exception
	{
		const char *	what() const throw();
	};
};
#endif
