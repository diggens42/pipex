#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <fcntl.h>
# include <sys/wait.h>
# include "libft.h"

typedef struct s_pipex
{
	int		infile;
	int		outfile;
	int		pipe_fd[2];
	char	**cmd_args;
	char	*cmd_path;
}	t_pipex;

void	error_exit(char *msg);
char	*find_command_path(char *cmd, char **envp);
char	**parse_cmd(char *arg);
void	child_one(t_pipex *pipex, char *cmd_arg, char **envp);

#endif
