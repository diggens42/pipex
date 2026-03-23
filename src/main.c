#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	(void)envp;
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
	close(pipex.pipe_fd[0]);
	close(pipex.pipe_fd[1]);
	close(pipex.infile);
	close(pipex.outfile);
	return (0);
}
