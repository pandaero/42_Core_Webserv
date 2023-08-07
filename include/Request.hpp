/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 22:04:57 by pandalaf          #+#    #+#             */
/*   Updated: 2023/08/06 11:25:58 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Request_HPP
# define Request_HPP

# include "webserv.hpp"

# include <utility>
# include <string>
# include <map>
# include <cstdlib>

// Forward declaration necessary here, even though included in webserv.hpp
typedef std::map<std::string, std::string> 	strMap;
class	Client;

class	Request
{
	public:
		Request();
		Request(std::string);
		
		std::string	headerValue(std::string) const;
		std::string	getFilename() const;
		
		std::string	method() const;
		std::string path() const;
		std::string	httpProtocol() const;
		int			contentLength() const;
		std::string	getContentType() const;

		bool		headComplete();
		bool		bodyComplete();

		void		addToBodyBytesWritten(size_t);

	private:
		strMap		createHeaderMap(std::string&, std::string, std::string, std::string);

		bool		_headComplete;
		size_t		_bodyBytesWritten;

		std::string	_method;
		std::string	_path;
		std::string	_protocol;
		int			_contentLength;
		std::string	_contentType;
		strMap		_headers;
};
#endif
