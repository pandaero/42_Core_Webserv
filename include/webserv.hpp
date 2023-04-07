/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pandalaf <pandalaf@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 01:51:20 by pandalaf          #+#    #+#             */
/*   Updated: 2023/04/07 20:26:05 by pandalaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# define RECEIVE_BUFFER 8192

enum serverParams
{
	NAME,
	ADDRESS,
	PORT,
	ROOT,
	MAXCONN
};

#endif
