/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pandalaf <pandalaf@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 16:01:24 by wmardin           #+#    #+#             */
/*   Updated: 2023/05/09 17:05:39 by pandalaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGFILE_HPP
# define CONFIGFILE_HPP

# include <fstream>
# include <sstream>
# include <iostream>

# include "webserv.hpp"
# include "ServerConfig.hpp"

class ConfigFile
{
	public:
		ConfigFile(const char*);
		~ConfigFile();

		std::vector<ServerConfig> getConfigs() const;

	private:
		std::string		fileToString(const char*);
		void			parseConfigData();
		std::string		getServerConfigElement();
		size_t			endOfElement(std::string);
		
		std::string					_configData;
		std::vector<ServerConfig>	_serverConfigs;
};

#endif
