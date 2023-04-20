#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

# include <fcntl.h>
# include <sys/wait.h>

void	ft_putstr_stderr(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		write(2, &str[i], 1);
		i++;
	}
}

void	show_error(char *msg)
{
	ft_putstr_stderr(msg);
	exit(EXIT_FAILURE);
}

void	fork_fail(int *fd)
{
	close(fd[0]);
	close(fd[1]);
	show_error("fork() failed.\n");
}

#define PORT 8080
int main(int argc, char const *argv[], char **envp)
{
    int server_fd, new_socket; long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char hello[1000];
    
    // Only this line has been changed. Everything is same.
    if (argc == 2) //cgi - here I want to pipe, execute and do everything CGI-related and redirect scripts output into the char * hello 
    {
        int     fd[2];
        pid_t   id;

        if (pipe(fd) == -1)
            show_error("pipe() failed.\n");
        id = fork();
        if (id == -1)
            fork_fail(fd);
        if (id == 0) //we're in the child process
        {
            close (fd[0]); //this is the read end
            if (dup2(fd[1], STDOUT_FILENO) == -1) //stdout == 1
                show_error("dup2() failed.\n");
            char    *args[] = {"/Users/apielasz/Documents/projects_git/mini_webserv/cgi-bin/first.cgi", NULL};

            execve(args[0], args, envp);
            show_error("execve() failed.");
        }
        else //we're in parent
        {
            close(fd[1]); //closing the write end
            waitpid(id, NULL, 0);
            //now we need to put the contents of the pipe into the char *, so read()
            ssize_t read_ret = read(fd[0], hello, 1000);
            close(fd[0]);
            if (read_ret == -1) 
                show_error("read() failed.\n");
            hello[read_ret] = '\0';
            printf("the read ret is: %s\n", hello);
        }
    }
    else
        strcpy(hello, "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 15\n\nbig momeeeeent!");
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    
    memset(address.sin_zero, '\0', sizeof address.sin_zero);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        
        char buffer[30000] = {0};
        valread = read( new_socket , buffer, 30000);
        printf("%s\n",buffer );
        write(new_socket , hello , strlen(hello));
        printf("------------------Hello message sent-------------------");
        close(new_socket);
    }
    return 0;
}
    