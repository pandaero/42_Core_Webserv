/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pandalaf <pandalaf@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 20:29:39 by pandalaf          #+#    #+#             */
/*   Updated: 2023/03/29 12:54:00 by pandalaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <exception>
# include <arpa/inet.h>
# include <sys/socket.h>

class	Client
{
	public:
		Client();
		Client(const Client &);
		~Client();
		Client &	operator=(const Client &);

		int		getSocketfd();

		void	accept(int);

	private:
		int					_clientSocketfd;
		socklen_t			_clientSockLen;
		struct sockaddr_in	_clientAddress;
	
	class	connectionDeniedException: public std::exception
	{
		const char *	what() const throw();
	};
};
#endif
