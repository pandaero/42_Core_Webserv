/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pandalaf <pandalaf@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 16:03:29 by wmardin           #+#    #+#             */
/*   Updated: 2023/05/09 17:38:08 by pandalaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ConfigFile.hpp"

ConfigFile::ConfigFile(const char* path)
{
	_configData = fileToString(path);
	parseConfigData(); 
}

ConfigFile::~ConfigFile()
{}

std::vector<ServerConfig> ConfigFile::getConfigs() const
{
	return _serverConfigs;
}

std::string ConfigFile::fileToString(const char* path)
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
	return buffer.str();
}

void ConfigFile::parseConfigData()
{
	trim(_configData);
	while (!_configData.empty())
	{
		ServerConfig newConfig(getServerConfigElement());
		_serverConfigs.push_back(newConfig);
	}
	std::cout << "Info: getConfigs: " << _serverConfigs.size() << (_serverConfigs.size() == 1 ? " config element built." : " config elements built.") << std::endl;
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
	
	//len_open = configString.find("{");

	
	// get subelement
	// while get subeleement
	//	parse subeleemtn
	len_close = endOfElement(_configData);
	elementBody = _configData.substr(0, len_close);
	_configData.erase(0, len_close + 1);
	//elementBody = splitEraseStr(configString, "}");
	std::cout << "elementbody: " << elementBody << std::endl;
	std::cout << "configString: " << _configData << std::endl;
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
	return i;
}
