#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	pid_t	pid1;
	pid_t	pid2;

	if (argc != 5)
	{
		write(2, "Usage: ./pipex file1 cmd1 cmd2 file2\n", 36);
		return (1);
	}
	pipex.infile = open(argv[1], O_RDONLY);
	if (pipex.infile < 0)
		error_exit(argv[1]);
	pipex.outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex.outfile < 0)
	{
		close(pipex.infile);
		error_exit(argv[4]);
	}
	if (pipe(pipex.pipe_fd) == -1)
	{
		close(pipex.infile);
		close(pipex.outfile);
		error_exit("pipe");
	}
	pid1 = fork();
	if (pid1 == -1)
	{
		close(pipex.pipe_fd[0]);
		close(pipex.pipe_fd[1]);
		close(pipex.infile);
		close(pipex.outfile);
		error_exit("fork");
	}
	if (pid1 == 0)
		child_one(&pipex, argv[2], envp);
	pid2 = fork();
	if (pid2 == -1)
	{
		close(pipex.pipe_fd[0]);
		close(pipex.pipe_fd[1]);
		close(pipex.infile);
		close(pipex.outfile);
		error_exit("fork");
	}
	if (pid2 == 0)
		child_two(&pipex, argv[3], envp);
	close(pipex.pipe_fd[0]);
	close(pipex.pipe_fd[1]);
	close(pipex.infile);
	close(pipex.outfile);
	return (0);
}
