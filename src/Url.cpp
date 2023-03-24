/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Url.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pandalaf <pandalaf@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 09:38:38 by pandalaf          #+#    #+#             */
/*   Updated: 2023/03/24 18:11:39 by pandalaf         ###   ########.fr       */
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

Url::Url(std::string locator):
	_rawString(),
	_properties()
{
	if (locator.empty() || locator.find(':') == std::string::npos)
		throw invalidUrlException();
	if (locator.find("//") == std::string::npos)
		throw invalidUrlException();

	std::pair<bool, std::string>				defaultPair(false, "");
	std::vector<std::pair<bool, std::string> >	properties(6);
	for (int i = 0; i < 6; ++i)
		properties.push_back(defaultPair);

	std::string::iterator	locIt = locator.begin();
	size_t	i = 0;

	while (locIt != locator.end() && *locIt++ != ':')
		++i;
	properties[SCHEME].first = true;
	properties[SCHEME].second = locator.substr(0, i);
	++i;

	if (*locIt++ != '/')
		throw invalidUrlException();
	++i;

	if (*locIt++ != '/')
		throw invalidUrlException();
	++i;

	if (*locIt == '/')
	{
		++i;
		size_t	pathStart = i;
		while (++locIt != locator.end() && (*locIt != '/' && locator.find('/', i) != std::string::npos ))
			++i;
		properties[PATH].first = true;
		properties[PATH].second = locator.substr(pathStart, i);
		if (locIt == locator.end())
		{
			new (this) Url(locator, properties);
			return;
		}
	}
	else
	{
		size_t	domainStart = i;
		while (++locIt != locator.end() && *locIt != ':' && *locIt != '/')
			++i;
		++i;
		properties[DOMAIN].first = true;
		properties[DOMAIN].second = locator.substr(domainStart, i - domainStart);
		std::string::iterator	endCheck = locIt;
		++endCheck;
		if (locIt == locator.end() || *endCheck == '/')
		{
			new (this) Url(locator, properties);
			return;
		}
		if (*locIt == ':')
		{
			++i;
			size_t	portStart = i;
			while (++locIt != locator.end() && *locIt != '/')
				++i;
			properties[PORT].first = true;
			properties[PORT].second = locator.substr(portStart, i - portStart);
			std::string::iterator	endCheck = locIt;
			++endCheck;
			if (locIt == locator.end() || endCheck == locator.end())
			{
				new (this) Url(locator, properties);
				return;
			}
		}
	}
	
	if (*locIt == '/')
	{
		++i;
		size_t	pathStart = i;
		while (locIt++ != locator.end() && *locIt != '?' && *locIt != '#')
			++i;
		properties[PATH].first = true;
		properties[PATH].second = locator.substr(pathStart, i - pathStart);
		if (locIt == locator.end())
		{
			new (this) Url(locator, properties);
			return;
		}
		if (*locIt == '?')
		{
			++i;
			size_t	paramStart = i;
			while (++locIt != locator.end() && *locIt != '#')
				++i;
			properties[PARAMETERS].first = true;
			properties[PARAMETERS].second = locator.substr(paramStart, i - paramStart);
			if (locIt == locator.end())
			{
				new (this) Url(locator, properties);
				return;
			}
			if (*locIt == '#')
			{
				++i;
				size_t	anchorStart = i;
				while (++locIt != locator.end())
					++i;
				properties[ANCHOR].first = true;
				properties[ANCHOR].second = locator.substr(anchorStart, i - anchorStart);
				new (this) Url(locator, properties);
				return;
			}
		}
		if (*locIt == '#')
		{
			++i;
			size_t	anchorStart = i;
			while (++locIt != locator.end())
				++i;
			properties[ANCHOR].first = true;
			properties[ANCHOR].second = locator.substr(anchorStart, i - anchorStart);
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

std::vector<std::pair<bool, std::string> >	Url::getProperties() const
{
	return (_properties);
}

const char *	Url::invalidUrlException::what() const throw()
{
	return ("Error: URL: invalid URL.");
}
