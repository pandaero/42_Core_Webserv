/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Url.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pandalaf <pandalaf@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 09:38:38 by pandalaf          #+#    #+#             */
/*   Updated: 2023/03/23 20:10:03 by pandalaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Url.hpp"

std::ostream &	operator<<(std::ostream & outStream, const Url & url)
{
	outStream << url.getStr();
	return (outStream);
}

Url::Url(std::string raw, std::vector<std::pair<bool, std::string> > properties):
	_rawString(raw),
	_properties(properties)
{

}

Url::Url()
{

}

Url::Url(const Url & other):
	_rawString(other._rawString),
	_properties(other._properties)
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
#include <iostream>
#include <unistd.h>
Url::Url(std::string locator):
	_rawString(locator),
	_properties()
{
	if (locator.empty() || locator.find(':') == std::string::npos)
		throw invalidUrlException();
	if (locator.find("//") == std::string::npos)
		throw invalidUrlException();

	std::pair<bool, std::string>	defavlt(false, "");
	std::pair<bool, std::string>	scheme(defavlt);
	std::pair<bool, std::string>	domain(defavlt);
	std::pair<bool, std::string>	port(defavlt);
	std::pair<bool, std::string>	path(defavlt);
	std::pair<bool, std::string>	parameters(defavlt);
	std::pair<bool, std::string>	anchor(defavlt);
	std::vector<std::pair<bool, std::string> >	properties(6);

	std::string::iterator	locIt = locator.begin();
	size_t	i = 0;
	while (locIt != locator.end() && *locIt++ != ':')
		++i;
	scheme.first = true;
	scheme.second = locator.substr(0, i);
	//Check valid scheme

	std::cout << "Scheme: " << scheme.second << ", locIt: " << *locIt << std::endl;

	if (*locIt != '/')
		throw invalidUrlException();

	while (*locIt++ == '/')
		++i;
	++i;
	std::cout << "locIt: " << *locIt << ", i: " << i <<  std::endl;

	size_t	domainStart = i;
	while (++locIt != locator.end() && *locIt != ':' && *locIt != '/')
		++i;

	std::cout << "locIt: " << *locIt << ", i: " << i << std::endl;
	i += 2;
	domain.first = true;
	domain.second = locator.substr(domainStart, i - domainStart);
	
	std::cout << "Domain: " << domain.second << ", locIt: " << *locIt << std::endl;

	if (locIt == locator.end())
	{
		properties[SCHEME] = scheme;
		properties[DOMAIN] = domain;
		properties[PORT] = port;
		properties[PATH] = path;
		properties[PARAMETERS] = parameters;
		properties[ANCHOR] = anchor;
		new (this) Url(locator, properties);
		return;
	}
	
	if (*locIt == ':')
	{
		size_t	portStart = i;
		while (locIt++ != locator.end() && *locIt != '/')
			++i;
		port.first = true;
		port.second = locator.substr(portStart, i);
	// std::cout << "Port: " << port.second << ", locIt: " << *locIt << std::endl;
		if (locIt == locator.end())
		{
			properties[SCHEME] = scheme;
			properties[DOMAIN] = domain;
			properties[PORT] = port;
			properties[PATH] = path;
			properties[PARAMETERS] = parameters;
			properties[ANCHOR] = anchor;
			new (this) Url(locator, properties);
			return;
		}
	}

	if (*locIt == '/')
	{
		size_t	pathStart = i;
		while (locIt++ != locator.end() && *locIt != '?' && *locIt != '#')
			++i;
		path.first = true;
		path.second = locator.substr(pathStart, i);
		if (locIt == locator.end())
		{
			properties[SCHEME] = scheme;
			properties[DOMAIN] = domain;
			properties[PORT] = port;
			properties[PATH] = path;
			properties[PARAMETERS] = parameters;
			properties[ANCHOR] = anchor;
			new (this) Url(locator, properties);
			return;
		} 
	}
	new (this) Url(locator, properties);

	// if (locator.find('#') != std::string::npos)
	// {
	// 	anchor.first = true;
	// 	anchor.second = locator.substr(locator.find('#'), locator.end());
	// }
	// else
	// 	anchor.first = false;

	// std::pair<bool, std::string>	parameters;
	// if (locator.find('?') != std::string::npos)
	// {
	// 	parameters.first = true;
	// 	if (anchor.first == true)
	// 		parameters.second = locator.substr(locator.find('?'), locator.find('#'));
	// 	else
	// 		parameters.second = locator.substr(locator.find('?'), locator.end());
	// }
	// else
	// 	parameters.first = false;

	// new (this) Url()
}

std::string	Url::getStr() const
{
	return (_rawString);
}

std::vector<std::pair<bool, std::string> >	Url::getProperties() const
{
	return (_properties);
}

const char *	Url::invalidUrlException::what() const throw()
{
	return ("Error: URL: invalid URL.");
}
