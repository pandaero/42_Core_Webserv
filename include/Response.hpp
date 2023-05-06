/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 17:00:19 by pandalaf          #+#    #+#             */
/*   Updated: 2023/05/06 19:26:46 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "webserv.hpp"
# include "Server.hpp"


# include <fstream>
# include <sstream>
# include <iostream>
# include <cstring>
# include <cstdlib>
# include <string>
# include <cstdio>
# include <sys/socket.h>

class	Response
{
	public:
		Response();
		Response(int, const Server &);
		~Response();

		void	setStatusCode(int);
	
		void	setFile(std::string);

		int		send(int);

	private:
		void		build();

		std::string _responseHeader;
		
		int			_statusCode;
		std::string	_statusMessage;

		contentType	_contentType;
		long		_fileSize;
		std::string	_filePath;
};
#endif
