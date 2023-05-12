/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 16:03:29 by wmardin           #+#    #+#             */
/*   Updated: 2023/05/12 20:48:49 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ConfigFile.hpp"

ConfigFile::ConfigFile(const char* path)
{
	// parse default ServerConfig object from internal default config file
	loadFile(PATH_DEFAULTCONFIG);
	ServerConfig	defaultConfig(getServerConfigElement());
	_defaultServerConfig = defaultConfig;
	std::cout << I_DEFAULTIMPORT << PATH_DEFAULTCONFIG << std::endl;
	
	// parse intended ServerConfig objects from client supplied config file
	loadFile(path);
	parseConfigData();
	//std::cout << I_CONFIGIMPORT << std::endl;
}

ConfigFile::~ConfigFile()
{}

std::vector<ServerConfig> ConfigFile::getConfigs() const
{
	return _serverConfigs;
}

void ConfigFile::loadFile(const char* path)
{
	std::ifstream		infile(path);
	std::string			line;
	std::stringstream	buffer;
	
	if (!infile)
	{
		std::string invalidpath(path);
		throw std::runtime_error(E_FILEOPEN + invalidpath + '\n');
	}
	while (std::getline(infile, line))
	{
		if (line[line.find_first_not_of(WHITESPACE)] == '#')
			continue;
		buffer << line;
	}
	infile.close();
	_configData = buffer.str();
	trim(_configData);
}

void ConfigFile::parseConfigData()
{
	while (!_configData.empty())
	{
		ServerConfig newConfig(_defaultServerConfig);
		newConfig.applySettings(getServerConfigElement());
		_serverConfigs.push_back(newConfig);
	}
	if (_serverConfigs.empty())
		throw std::runtime_error(E_NOSERVER);
}

std::string ConfigFile::getServerConfigElement()
{
	std::string		elementTitle, elementBody;
	size_t			len_close;
	
	elementTitle = splitEraseStr(_configData, "{");
	if (trim(elementTitle) != SERVER)
		throw std::runtime_error(E_ELMNTDECL + elementTitle + '\n');
	len_close = endOfElement(_configData);
	if (len_close == (size_t)-1)
		throw std::runtime_error(E_INVALBRACES + _configData + '\n');
	elementBody = _configData.substr(0, len_close - 1);
	_configData.erase(0, len_close);
	return trim(elementBody);
}

size_t ConfigFile::endOfElement(std::string elementBody)
{
	size_t	i, counter;

	i = 0;
	counter = 1;
	for (; elementBody[i] && counter > 0; i++)
	{
		if (elementBody[i] == '{')
			counter++;
		if (elementBody[i] == '}')
			counter--;
	}
	if (counter > 0)
		return -1;
	return i;
}
