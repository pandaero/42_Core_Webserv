/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pandalaf <pandalaf@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 17:00:19 by pandalaf          #+#    #+#             */
/*   Updated: 2023/04/30 23:35:10 by pandalaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "webserv.hpp"

# include <string>
# include <cstdio>
# include <sys/socket.h>

class	Response
{
	public:
		Response();
		~Response();

		void	setStatusCode(int);
	
		void	setFile(std::string);

		int		sendResponse(int);

	private:
		void		buildResponse();

		std::string _responseHeader;
		
		int			_statusCode;
		std::string	_statusMessage;

		contentType	_contentType;
		long		_fileSize;
		std::string	_filePath;
};
#endif
