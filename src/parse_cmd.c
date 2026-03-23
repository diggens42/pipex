#include "pipex.h"

char	**parse_cmd(char *arg)
{
	char	**cmd;

	cmd = ft_split(arg, ' ');
	if (!cmd)
		error_exit("malloc");
	return (cmd);
}
