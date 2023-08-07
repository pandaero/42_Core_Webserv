/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 20:29:39 by pandalaf          #+#    #+#             */
/*   Updated: 2023/08/06 21:51:07 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "webserv.hpp"
# include "Request.hpp"

# include <sstream>
# include <exception>
# include <arpa/inet.h>
# include <sys/socket.h>

class	Client
{
	public:
		Client(int, int);

		sockaddr_in*	getSockaddr();
		int				getSocketfd() const;
		int				getPollStructIndex() const;
		
		void			setClientSocketfd(int);

		void			appendToBuffer(std::string, int);
		void			handleRequest();

		void			resetData();
		
		bool			RequestComplete();
		bool			requestBodyComplete();
		
		Request		_Request;
		std::string		_requestbody;

	private:
		std::string		_buffer;
		int				_bodyBytesRead;
		
		bool			_RequestComplete;
		bool			_requestBodyComplete;
		
		int				_serverSocketfd;
		int				_clientSocketfd;
		int				_pollStructIndex;
		sockaddr_in		_clientAddress;
};

#endif
