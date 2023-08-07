/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 17:00:19 by pandalaf          #+#    #+#             */
/*   Updated: 2023/08/06 21:51:07 by wmardin          ###   ########.fr       */
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
# include <sys/stat.h>
# include <sys/socket.h>

class	Server;

class	Response
{
	public:
		Response();
		Response(int);
		Response(const Request &, const Server &);
		~Response();

		void			setFile(std::string, const Server &);

		int				send(int, const Server &);
		
		const char*		getStatusPage();

		std::string		getFilePath();
		off_t			getSize();
		
		std::string 	_responseHeader;
		int				_statusCode;
		std::string		_statusMessage;

	private:
		void			build();
		std::string		getHttpMsg(int);
		
		std::string		_statusPage;
		contentType		_contentType;
		off_t			_fileSize;
		std::string		_filePath;
};
#endif
