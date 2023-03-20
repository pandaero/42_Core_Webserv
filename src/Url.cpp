/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Url.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pandalaf <pandalaf@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 09:38:38 by pandalaf          #+#    #+#             */
/*   Updated: 2023/03/20 09:46:52 by pandalaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Url.hpp"

std::ostream &	operator<<(std::ostream & outStream, const Url & url)
{
	outStream << url.getStr();
	return (outStream);
}

Url::Url()
{

}

Url::Url(const Url & other):
	_rawString(other._rawString),
	_scheme(other._scheme),
	_domain(other._domain),
	_port(other._port),
	_path(other._path),
	_parameters(other._parameters),
	_anchor(other._anchor)
{

}

Url::~Url()
{

}

Url & Url::operator=(const Url & other)
{
	if (this == &other)
	{
		new (this) Url(other);
	}
	return (*this);
}

Url::Url(std::string)
{
	//PARSE and fill fields
	//IF NOT VALID THROW exception
}

std::string	Url::getStr() const
{
	return (_rawString);
}

const char *	Url::invalidUrlException::what() const throw()
{
	return ("Error: URL: invalid URL.");
}
