/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pandalaf <pandalaf@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 22:04:57 by pandalaf          #+#    #+#             */
/*   Updated: 2023/05/01 23:25:38 by pandalaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "webserv.hpp"

# include <utility>

typedef std::vector<std::pair<std::string, std::string> > stringPairVec;

class	Request
{
	public:
		Request(std::string);
		std::string	headerValue(std::string);

		std::string	path();

	private:
		stringPairVec	_headers;
};
#endif
