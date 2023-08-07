/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apielasz <apielasz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 16:59:02 by apielasz          #+#    #+#             */
/*   Updated: 2023/08/07 17:04:07 by apielasz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

#include <string>
#include <iostream>

class CGI {
	public:
		CGI();
		~CGI();
		
		bool	handle_env(std::string path_to_script);

	private:
		char	**_env;
}
