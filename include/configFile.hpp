/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 16:01:24 by wmardin           #+#    #+#             */
/*   Updated: 2023/05/09 16:43:03 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGFILE_HPP
# define CONFIGFILE_HPP

# include <fstream>
# include <sstream>
# include <iostream>

# include "webserv.hpp"
# include "ServerConfig.hpp"

class configFile
{
	public:
		configFile(const char*);
		~configFile();

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
