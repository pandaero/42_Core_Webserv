/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Url.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pandalaf <pandalaf@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 20:15:58 by pandalaf          #+#    #+#             */
/*   Updated: 2023/03/20 09:46:54 by pandalaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef URL_HPP
# define URL_HPP

# include <string>
# include <ostream>
# include <exception>

class	Url;

std::ostream &	operator<<(std::ostream &, const Url &);

class	Url
{
	private:
		const std::string	_rawString;
		const std::string	_scheme;
		const std::string	_domain;
		const std::string	_port;
		const std::string	_path;
		const std::string	_parameters;
		const std::string	_anchor;

	public:
		Url();
		Url(const Url &);
		~Url();
		Url &	operator=(const Url &);

		Url(std::string);

		std::string	getStr() const;

	class	invalidUrlException(): public std::exception
	{
		const char *	what() const throw();
	};
};
#endif
