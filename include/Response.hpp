/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pandalaf <pandalaf@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 17:00:19 by pandalaf          #+#    #+#             */
/*   Updated: 2023/04/01 17:57:34 by pandalaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string>

class	Response
{
	public:
		Response();
		Response(const Response &);
		~Response();
		Response &	operator=(const Response &);

		void	setStatusCode(int);
	
		void	loadPage(std::string);

		void	buildResponse();

		const char *	send_msg();
		int		send_size();

	private:
		char *		_fullResponseC;
		std::string _fullResponse;
		int			_statusCode;
		std::string	_statusMessage;
		std::string	_webPage;
};
#endif
