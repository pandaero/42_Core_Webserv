/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pandalaf <pandalaf@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 22:04:57 by pandalaf          #+#    #+#             */
/*   Updated: 2023/05/06 17:37:55 by pandalaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "webserv.hpp"

# include <utility>
# include <string>
# include <map>
# include <cstdlib>

class	Request
{
	public:
		friend class	Server;
		
		Request();
		Request(std::string);
		std::string	headerValue(std::string);

		std::string	getFile();

	private:
		std::string		_method;
		std::string		_path;
		std::string		_protocol;
		int				_contentLength;

		std::map<std::string, std::string> _headers;
};
#endif
