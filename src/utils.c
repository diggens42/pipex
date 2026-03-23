#include "pipex.h"

void	error_exit(char *msg)
{
	perror(msg);
	exit(1);
}
