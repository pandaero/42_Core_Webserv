/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Url.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pandalaf <pandalaf@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 09:38:38 by pandalaf          #+#    #+#             */
/*   Updated: 2023/03/25 22:59:31 by pandalaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Url.hpp"

#include <algorithm>

std::ostream &	operator<<(std::ostream & outStream, const Url & url)
{
	outStream << url.getStr();
	return (outStream);
}

Url::Url(std::string raw, std::vector<std::string> properties):
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

Url::Url(std::string locator):
	_rawString(),
	_properties()
{
	if (locator.empty() || locator.find(':') == std::string::npos)
		throw invalidUrlException();
	if (locator.find("//") == std::string::npos)
		throw invalidUrlException();

	std::vector<std::string>	properties(6);
	for (int i = 0; i < 6; ++i)
		properties.push_back("");

	size_t	i = 0;

	while (i < locator.size() && locator[i] != ':')
	{
		if (locator[i] == '/')
			throw invalidUrlException();
		++i;
	}
	properties[SCHEME] = locator.substr(0, i);


	if (locator[++i] != '/')
		throw invalidUrlException();
	
	if (locator[i++] != '/')
		throw invalidUrlException();
	
	if (locator[++i] == '/')
	{
		size_t	pathStart = i;
		while (i < locator.size() && (locator[i] != '/' && locator.find('/', i) != std::string::npos))
			++i;
		properties[PATH] = locator.substr(pathStart, i);
		if (i == locator.size())
		{
			new (this) Url(locator, properties);
			return;
		}
	}
	else
	{
		size_t	domainStart = i;
		while (i < locator.size() && locator[i] != ':' && locator[i] != '/')
			++i;
		properties[DOMAJN] = locator.substr(domainStart, i - domainStart);
		size_t	j = i + 1;
		if (i == locator.size() || locator[j] == '/')
		{
			new (this) Url(locator, properties);
			return;
		}
		if (locator[i] == ':')
		{
			++i;
			size_t	portStart = i;
			while (i < locator.size() && locator[i] != '/')
				++i;
			properties[PORT] = locator.substr(portStart, i - portStart);
			if (i > locator.size())
			{
				new (this) Url(locator, properties);
				return;
			}
		}
	}
	
	if (locator[i] == '/')
	{
		++i;
		size_t	pathStart = i;
		while (i != locator.size() && locator[i] != '?' && locator[i] != '#')
			++i;
		properties[PATH] = locator.substr(pathStart, i - pathStart);
		if (i == locator.size())
		{
			new (this) Url(locator, properties);
			return;
		}
		if (locator[i] == '?')
		{
			++i;
			size_t	paramStart = i;
			while (i < locator.size() && locator[i] != '#')
				++i;
			properties[PARAMETERS] = locator.substr(paramStart, i - paramStart);
			if (i == locator.size())
			{
				new (this) Url(locator, properties);
				return;
			}
			if (locator[i] == '#')
			{
				size_t	anchorStart = i + 1;
				while (i < locator.size())
					++i;
				properties[ANCHOR] = locator.substr(anchorStart, i - anchorStart);
				new (this) Url(locator, properties);
				return;
			}
		}
		if (locator[i] == '#')
		{
			size_t	anchorStart = i + 1;
			while (i < locator.size())
				++i;
			properties[ANCHOR] = locator.substr(anchorStart, i - anchorStart);
			new (this) Url(locator, properties);
			return;
		}
	}
	new (this) Url(locator, properties);
}

std::string	Url::getStr() const
{
	return (_rawString);
}

std::vector<std::string>	Url::getProperties() const
{
	return (_properties);
}

const char *	Url::invalidUrlException::what() const throw()
{
	return ("Error: URL: invalid URL.");
}
