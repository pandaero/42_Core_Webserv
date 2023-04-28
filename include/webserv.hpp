/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 01:51:20 by pandalaf          #+#    #+#             */
/*   Updated: 2023/04/28 16:34:26 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream>
# include <vector>
# include <map>

# define RECEIVE_BUFFER 8192

// Main settings
# define SERVERNAME	"server_name"
# define HOST		"host"
# define PORT		"listen"

// Bools
# define GET		"GET"
# define POST		"POST"
# define DELETE		"DELETE"
# define DIRLISTING	"directoryListing"

// Directories
# define ROOT		"root"
# define DIR		"dir"
# define UPLOADDIR	"uploadDir"
# define CGIDIR		"CGIdir"
# define ERRORPAGE	"errorPage"

// Size limits
# define CLIMAXBODY	"clientMaxBodySize"
# define MAXCONNS	"maxConnections"
# define BACKLOG	"backlog"

// Errors
# define E_FILEOPEN		"Could not open config file: "
# define E_NOSERVER		"No valid server configs found."
# define E_ELMNTDECL	"Invalid element declaration, (only \"server\" allowed): "
# define E_SUBELEMNT	"Subelements not allowed: "

typedef std::map<std::string, std::string>::const_iterator	StringMap_it;
typedef std::map<std::string, std::string> 					StringMap;

bool 			isAlnumString(const std::string&);
std::string		trim(std::string& input);
std::string		splitEraseStr(std::string&, std::string);
StringMap		splitEraseStrMap(std::string&, std::string, std::string, char);


#endif
