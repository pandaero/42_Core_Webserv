/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialinaok <ialinaok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 22:34:49 by ialinaok          #+#    #+#             */
/*   Updated: 2023/08/07 23:12:11 by ialinaok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/CGI.hpp"

//CGI script will create entire http response

CGI::CGI() {}
CGI::~CGI() {}

CGI::CGI(std::string pathToScript, std::string method, std::string queries) : _path(pathToScript), _method(method), _queries(queries) {}

// we'll have 2 scripts I think - 1 php that work woth POST and GET
// and one py to get the bonus
// it would be cool if these two scripts took the same variables for env...
// OK so first I need to establish the environment right
// in my script I have $_SERVER['REQUEST_METHOD'] $_FILES['file']

int	CGI::doTheThing() {
	
	
}