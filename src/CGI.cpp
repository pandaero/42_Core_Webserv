/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apielasz <apielasz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 22:34:49 by ialinaok          #+#    #+#             */
/*   Updated: 2023/08/08 17:10:11 by apielasz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/CGI.hpp"

//CGI script will create entire http response
//do we add recognition of the system on which we are or assume MacOS?

CGI::~CGI() {}

CGI::CGI(Client &client) : _clientInfos(client) {}

// we'll have 2 scripts I think - 1 php that work woth POST and GET
// and one py to get the bonus
// it would be cool if these two scripts took the same variables for env...
// OK so first I need to establish the environment right
// in my script I have $_SERVER['REQUEST_METHOD'] $_FILES['file']

int	CGI::fillEnv() {
	
	std::vector<std::string>	tmpEnv;
	std::string	tmpVar;

// env variables that are not request specific
	//this one can be anything bc we have our own webserv
	tmpVar = "SERVER_SOFTWARE=webservInC++98BabyLetsGo/4.2";
	tmpEnv.push_back(tmpVar);
	//server's hostname or IP address
	tmpVar = "SERVER_NAME=myserver";//🍏
	tmpEnv.push_back(tmpVar);
	tmpVar = "GATEWAY_INTERFACE=CGI/1.2";
	tmpEnv.push_back(tmpVar);

// env variables specific to the request
	tmpVar = "SERVER_PROTOCOL=" + _clientInfos._request.httpProtocol();
	tmpEnv.push_back(tmpVar);
	//port number to which request was sent
	tmpVar = "SERVER_PORT=8080";//🍏
	tmpEnv.push_back(tmpVar);
	tmpVar = "REQUEST_METHOD=" + _clientInfos._request.method();
	tmpEnv.push_back(tmpVar);
	tmpVar = "PATH_INFO=" + _clientInfos.path();
	tmpEnv.push_back(tmpVar);
	tmpVar = "SCRIPT_NAME=" + _clientInfos._request.path();
	tmpEnv.push_back(tmpVar);
	tmpVar = "QUERY_STRING=";//🍏
	tmpEnv.push_back(tmpVar);
	tmpVar = "CONTENT_TYPE=" + _clientInfos._request.getContentType();
	tmpEnv.push_back(tmpVar);
	tmpVar = "CONTENT_LENGTH=" + _clientInfos._request.contentLength();
	tmpEnv.push_back(tmpVar);

//putting vector into char **
	int	i = 0;
	for (std::vector<std::string>::iterator it = tmpEnv.begin(); it != tmpEnv.end(); ++it) {
		_env[i] = const_cast<char *>((*it).c_str());
		i++;
	}
	_env[i] = NULL;
}

int	CGI::doTheThing() {
	
	std::string	pathToScript = _clientInfos._request.path();
	std::string pathToExec = "default/cgi/php-cgi-8.2.5_MacOS-10.15";
//creating a name for outfile with http request
	clientID = 12345;//🍏
	std::ostringstream oss;
	oss << clientID;
	std::string	outfilePath = "/cgi-bin/req/outfile" + clientID;

	int		pipeFd[2];

	if (pipe(pipeFd) == -1) {
		std::cerr << "CGI error: pipe() failed" << std::endl;
		throw CGIerrorException();
	}

	pid_t	pid = fork();

	if (pid == -1) {
		std::cerr << "CGI error: fork() failed" << std::endl;
		throw CGIerrorException();
	}
	if (pid == 0) {
//creating output file
		std::ofstream	outfile(outfilePath);
		if (!outfile.is_open()) {
			std::cerr << "CGI error: creating outfile failed" << std::endl;
			throw CGIerrorException();
		}
//duping the outfile
		close(pipeFd[0]);
		if (dup2(outfile.fileno(), STDOUT_FILENO) == -1) {
			outfile.close();
			std::cerr << "CGI error: dup2() failed" << std::endl;
			throw CGIerrorException();
		}
		outfile.close();
//creating argv for execve
		char	*argv[3];
		argv[0] = pathToExec;
		argv[1] = pathToScript;
		argv[2] = NULL;
		execve(pathToExec, argv, _env);
		std::cerr << "CGI error: execve() failed" << std::endl;
		throw CGIerrorException();
	} else {
		close(pipeFd[1]);
		close(pipeFd[0]);
	}
}

int	CGI::timeoutKillChild(pid_t childPid, int timeoutSec) {
	
	sleep(timeoutSec);
	int	killSuccessful = kill(childPid, SIGTERM);

	if (killSuccessful == 0) {
		std::cerr << "CGI error: TIMEOUT" << std::endl;
	} else {
		std::cerr << "CGI error: TIMEOUT failed to kill" << std::endl;
	}
}