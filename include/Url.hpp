/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Url.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pandalaf <pandalaf@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 20:15:58 by pandalaf          #+#    #+#             */
/*   Updated: 2023/03/23 01:15:37 by pandalaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef URL_HPP
# define URL_HPP

# include <string>
# include <vector>
# include <ostream>
# include <utility>
# include <exception>

enum	e_urlProperties
{
	SCHEME,
	DOMAIN,
	PORT,
	PATH,
	PARAMETERS,
	ANCHOR
};

class	Url;

std::ostream &	operator<<(std::ostream &, const Url &);

class	Url
{
	private:
		Url(std::string, std::vector<std::pair<bool, std::string> >);
		const std::string									_rawString;
		const std::vector<std::pair<bool, std::string> >	_properties;

	public:
		Url();
		Url(const Url &);
		~Url();
		Url &	operator=(const Url &);

		Url(std::string);

		std::string									getStr() const;
		std::vector<std::pair<bool, std::string> >	getProperties() const;

	class	invalidUrlException: public std::exception
	{
		const char *	what() const throw();
	};
};
#endif
