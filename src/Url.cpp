/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Url.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pandalaf <pandalaf@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 09:38:38 by pandalaf          #+#    #+#             */
/*   Updated: 2023/03/20 22:30:05 by pandalaf         ###   ########.fr       */
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

Url::Url(std::string link)
{
	if (link.empty())
		throw invalidUrlException();
	std::pair<bool, std::string>	anchor;
	if (link.find('#') != std::string::npos)
	{
		anchor.first = true;
		anchor.second = link.substr(link.find('#'), link.end());
	}
	else
		anchor.first = false;

	std::pair<bool, std::string>	parameters;
	if (link.find('?') != std::string::npos)
	{
		parameters.first = true;
		if (anchor.first == true)
			parameters.second = link.substr(link.find('?'), link.find('#'));
		else
			parameters.second = link.substr(link.find('?'), link.end());
	}
	else
		parameters.first = false;




	std::string::iterator	firstColonIt = link.find(':');
	scheme = link.substr(firstColonIt);
	for (std::string::iterator it = link.begin(); it != link.end(); ++it)
	{
		
	}
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
