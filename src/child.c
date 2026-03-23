#include "pipex.h"

void	child_one(t_pipex *pipex, char *cmd_arg, char **envp)
{
	char	**cmd;
	char	*path;

	dup2(pipex->infile, STDIN_FILENO);
	dup2(pipex->pipe_fd[1], STDOUT_FILENO);
	close(pipex->pipe_fd[0]);
	close(pipex->pipe_fd[1]);
	close(pipex->infile);
	close(pipex->outfile);
	cmd = parse_cmd(cmd_arg);
	path = find_command_path(cmd[0], envp);
	if (!path)
	{
		ft_free_strarray(cmd);
		error_exit(cmd_arg);
	}
	execve(path, cmd, envp);
	free(path);
	ft_free_strarray(cmd);
	error_exit("execve");
}

void	child_two(t_pipex *pipex, char *cmd_arg, char **envp)
{
	char	**cmd;
	char	*path;

	dup2(pipex->pipe_fd[0], STDIN_FILENO);
	dup2(pipex->outfile, STDOUT_FILENO);
	close(pipex->pipe_fd[0]);
	close(pipex->pipe_fd[1]);
	close(pipex->infile);
	close(pipex->outfile);
	cmd = parse_cmd(cmd_arg);
	path = find_command_path(cmd[0], envp);
	if (!path)
	{
		ft_free_strarray(cmd);
		error_exit(cmd_arg);
	}
	execve(path, cmd, envp);
	free(path);
	ft_free_strarray(cmd);
	error_exit("execve");
}
