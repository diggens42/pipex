#ifndef PIPEX_H
# define PIPEX_H

#include <unistd.h>	// close, read, write, dup, dup2, access, execve, fork, pipe, unlink
#include <fcntl.h>	// open
#include <stdlib.h>	// exit, malloc, free
#include <stdio.h>	// perror
#include <string.h>	// strerror
#include <sys/wait.h> // waitpid, wait

typedef struct	s_px {
	int		in;
	int		out;
	int		fd[2];
	char	**args;
	char	*path;
}	t_px;

#endif