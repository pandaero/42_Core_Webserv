/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 22:04:57 by pandalaf          #+#    #+#             */
/*   Updated: 2023/05/06 17:24:16 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "webserv.hpp"

# include <utility>
# include <string>
# include <map>

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
