/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi-main.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apielasz <apielasz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 13:31:00 by apielasz          #+#    #+#             */
/*   Updated: 2023/08/08 16:44:13 by apielasz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/CGI.hpp"

int	main() {

	std::vector<std::string>	tmpEnv;
	std::string					tmpVar;
	char						*_env[12];

// env variables that are not request specific
	//this one can be anything bc we have our own webserv
	tmpVar = "SERVER_SOFTWARE=webservInC++98BabyLetsGo/4.2";
	tmpEnv.push_back(tmpVar);
	//server's hostname or IP address
	std::ostringstream oss;
	oss << 55;
	tmpVar = "SERVER_NAME=myserver" + oss.str();//🤠
	tmpEnv.push_back(tmpVar);
	tmpVar = "GATEWAY_INTERFACE=CGI/1.2";
	tmpEnv.push_back(tmpVar);

//putting vector into char **
	int	i = 0;
	for (std::vector<std::string>::iterator it = tmpEnv.begin(); it != tmpEnv.end(); ++it) {
		_env[i] = const_cast<char *>((*it).c_str());
		i++;
	}
	_env[i] = NULL;
	i = 0;
	while (_env[i] != NULL) {
		std::cout << i << ". " << _env[i] << std::endl;
		i++;
	}
}

// int	main() {

// 	// execute the uname command and redirect output to a file
// 	int	result = std::system("uname > system_info.txt");

// 	if (result == 0) {
// 		std::cout << "System information written to system_info.txt" << std::endl;
// 	} else {
// 		std::cout << "Error executing command" << std::endl;
// 	}

// 	return(0);
// }