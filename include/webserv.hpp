/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pandalaf <pandalaf@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 01:51:20 by pandalaf          #+#    #+#             */
/*   Updated: 2023/04/07 20:26:05 by pandalaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream>
# include <vector>
# include <map>

# define RECEIVE_BUFFER 8192

enum serverParams
{
	NAME,
	ADDRESS,
	PORT,
	ROOT,
	MAXCONN
};

typedef std::map<std::string, std::string>::const_iterator	StringMap_it;
typedef std::map<std::string, std::string> 					StringMap;

bool 			isAlnumString(const std::string&);
std::string		trim(std::string& input);
std::string		splitEraseStr(std::string&, std::string);
StringMap		splitEraseStrMap(std::string&, std::string, std::string, char);


#endif
