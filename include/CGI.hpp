/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialinaok <ialinaok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 16:59:02 by apielasz          #+#    #+#             */
/*   Updated: 2023/08/08 22:14:32 by ialinaok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

#include <string>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "Request.hpp"
#include "Client.hpp"


class CGI {
	public:
		CGI(Client &);
		~CGI();

		int		doTheThing();
		int		fillEnv();
		int		timeoutKillChild(pid_t, int);

		class	CGIerrorException : public std::exception {
				virtual const char	*what() const throw();
		};

	private:
		char	*_env[12];
		Client	&_clientInfos;
		// std::string	_path;
		// std::string _method;
		// std::string _queries;
};

#endif