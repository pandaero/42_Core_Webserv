/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialinaok <ialinaok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 16:59:02 by apielasz          #+#    #+#             */
/*   Updated: 2023/08/07 23:10:57 by ialinaok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

#include <string>
#include <iostream>

class CGI {
	public:
		CGI();
		CGI(std::string, std::string, std::string);
		~CGI();

		int		doTheThing();
		bool	handleEnv(std::string);

	private:
		char	**_env;
		std::string	_path;
		std::string _method;
		std::string _queries;
};

#endif