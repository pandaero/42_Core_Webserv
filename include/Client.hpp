/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 20:29:39 by pandalaf          #+#    #+#             */
/*   Updated: 2023/08/07 13:56:42 by wmardin          ###   ########.fr       */
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
		Client(int);

		sockaddr_in*	sockaddr();
		int				socketfd() const;
		int				pollStructIndex() const;
		
		void			setSocketfd(int);

		void			handleRequestHead();
		void			handleRequestBody();

		void			writeToBuffer(std::string, int);
		void			writeToFile();

		bool			requestHeadComplete();
		bool			requestBodyComplete();
		
		Request			_request;

	private:
		std::string		_buffer;
		
		int				_clientSocketfd;
		int				_pollStructIndex;
		sockaddr_in		_clientAddress;
};

#endif
