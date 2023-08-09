/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi-main-testing.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apielasz <apielasz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 13:31:00 by apielasz          #+#    #+#             */
/*   Updated: 2023/08/09 19:56:53 by apielasz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../include/CGI.hpp"

#include <string>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <vector>

//here I just want to test functions, I don't want to create an object

int	timeoutKillChild(pid_t childPid, int timeoutSec) {
	
	sleep(timeoutSec);
	int	killSuccessful = kill(childPid, SIGTERM);

	if (killSuccessful == 0) {
		std::cerr << "CGI error: TIMEOUT" << std::endl;
		return (-1);
	} else {
		std::cerr << "CGI error: TIMEOUT, but failed to kill" << std::endl;
		return (-1);
	}
	return (0);
}

int	doTheThing() {
	
	std::string	pathToScript = "/Users/apielasz/Documents/projects_git/webserv/cgi-bin/script.py";
	std::string pathToExec = "/usr/bin/python3";

	int		pipeFd[2];

	if (pipe(pipeFd) == -1) {
		std::cerr << "CGI error: pipe() failed" << std::endl;
		return (-1);
	}

	pid_t	pid = fork();

	if (pid == -1) {
		std::cerr << "CGI error: fork() failed" << std::endl;
		return (-1);
	}
	if (pid == 0) {
		close(pipeFd[0]);
		if (dup2(pipeFd[1], STDOUT_FILENO) == -1) {
			std::cerr << "CGI error: dup2() failed" << std::endl;
			return (-1);
		}
//calling env filler - it's only needed in child process
		// fillEnv();
		std::vector<std::string>	tmpEnv;
		std::string	tmpVar;
		char	*_env[12];

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
		tmpVar = "SERVER_PROTOCOL=HTTP/1.1";
		tmpEnv.push_back(tmpVar);
		//port number to which request was sent
		tmpVar = "SERVER_PORT=8080";//🍏
		tmpEnv.push_back(tmpVar);
		tmpVar = "REQUEST_METHOD=GET";
		tmpEnv.push_back(tmpVar);
		tmpVar = "PATH_INFO=cgi-bin/script.php";
		tmpEnv.push_back(tmpVar);
		tmpVar = "SCRIPT_NAME=cgi-bin/script.php";
		tmpEnv.push_back(tmpVar);
		tmpVar = "QUERY_STRING=name=Alina&age=24";//🍏
		tmpEnv.push_back(tmpVar);
		tmpVar = "CONTENT_TYPE=";
		tmpEnv.push_back(tmpVar);
		tmpVar = "CONTENT_LENGTH=";
		tmpEnv.push_back(tmpVar);

	//putting vector into char **
		int	i = 0;
		for (std::vector<std::string>::iterator it = tmpEnv.begin(); it != tmpEnv.end(); ++it) {
			_env[i] = const_cast<char *>((*it).c_str());
			i++;
		}
		_env[i] = NULL;

//creating argv for execve
		const char	*argv[3];
		argv[0] = pathToExec.c_str();
		argv[1] = pathToScript.c_str();
		argv[2] = NULL;
		execve(argv[0], const_cast<char *const *>(argv), _env);
		std::cerr << "CGI error: execve() failed" << std::endl;
		return (-1);
	} else {
		close(pipeFd[1]);
		if (timeoutKillChild(pid, 10) == -1) {
			close(pipeFd[0]);
			return (-1);
		}
//only if there wasnt timeout we get here
		char	buffer[1024];
		ssize_t	bytesRead;
		std::string	cgiResponse;
		while ((bytesRead = read(pipeFd[0], buffer, 1023)) > 0) {
			buffer[bytesRead] = '\0';
			cgiResponse += buffer;
		}
		// std::cout << bytesRead << " and buffer:" << buffer << std::endl;
		close(pipeFd[0]);
		int	status;
		waitpid(pid, &status, 0);
		// int	killSuccessful = kill(pid, SIGTERM);
		
		std::cout << "Child process exited with status: " << WEXITSTATUS(status) << std::endl;
		std::cout << "SO THAT'S THE RESPONSE\n" << cgiResponse << std::endl;
	}
	return (0);
//so here I have the cgiResponse string that needs to be sent to the client 🍏
}


int	main() {

	doTheThing();
}

// int	main() {

// 	std::vector<std::string>	tmpEnv;
// 	std::string					tmpVar;
// 	char						*_env[12];

// // env variables that are not request specific
// 	//this one can be anything bc we have our own webserv
// 	tmpVar = "SERVER_SOFTWARE=webservInC++98BabyLetsGo/4.2";
// 	tmpEnv.push_back(tmpVar);
// 	//server's hostname or IP address
// 	std::ostringstream oss;
// 	oss << 55;
// 	tmpVar = "SERVER_NAME=myserver" + oss.str();//🤠
// 	tmpEnv.push_back(tmpVar);
// 	tmpVar = "GATEWAY_INTERFACE=CGI/1.2";
// 	tmpEnv.push_back(tmpVar);

// //putting vector into char **
// 	int	i = 0;
// 	for (std::vector<std::string>::iterator it = tmpEnv.begin(); it != tmpEnv.end(); ++it) {
// 		_env[i] = const_cast<char *>((*it).c_str());
// 		i++;
// 	}
// 	_env[i] = NULL;
// 	i = 0;
// 	while (_env[i] != NULL) {
// 		std::cout << i << ". " << _env[i] << std::endl;
// 		i++;
// 	}
// }

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