/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pandalaf <pandalaf@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 01:51:20 by pandalaf          #+#    #+#             */
/*   Updated: 2023/03/26 04:58:29 by pandalaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <arpa/inet.h>

# define SOCKETMAXCONNECTIONS 10

// Struct contains information about a client to respond to.
struct s_client_info
{
	int					fd;
	struct sockaddr_in	address;
	socklen_t			length;
};
#endif
