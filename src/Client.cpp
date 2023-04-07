/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pandalaf <pandalaf@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 20:51:05 by pandalaf          #+#    #+#             */
/*   Updated: 2023/04/07 20:19:53 by pandalaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

#include <unistd.h>

Client::Client(int serverSocketfd)
{
	_clientSocketfd = accept(serverSocketfd, (struct sockaddr *) &_clientAddress, &_clientSockLen);
	if (_clientSocketfd == -1)
	{
		close(_clientSocketfd);
		throw connectionDeniedException();
	}
}

int	Client::getSocketfd()
{
	return (_clientSocketfd);
}

const char *	Client::connectionDeniedException::what() const throw()
{
	return ("Error: connection with the client was denied.");
}
