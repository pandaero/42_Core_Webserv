/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 20:29:39 by pandalaf          #+#    #+#             */
/*   Updated: 2023/07/31 17:57:07 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "webserv.hpp"
# include "RequestHead.hpp"

# include <sstream>
# include <exception>
# include <arpa/inet.h>
# include <sys/socket.h>

class	Client
{
	public:
		Client(int, int);
		~Client();

		int				getSocketfd();
		int				getPollStructIndex();
		void			resetData();
		void			connect();
		
		bool			_gotRequestHead;
		std::string		_buffer;
		RequestHead		_requestHead;
		int				_bodyBytesRead;

	private:
		int				_serverSocketfd;
		int				_clientSocketfd;
		int				_pollStructIndex;
		socklen_t		_clientSockLen;
		sockaddr_in		_clientAddress;

	class	connectionDeniedException: public std::exception
	{
		const char *	what() const throw();
	};
};
#endif
