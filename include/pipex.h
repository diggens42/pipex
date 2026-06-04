/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 12:21:47 by fwahl             #+#    #+#             */
/*   Updated: 2026/06/04 12:21:47 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

#include <unistd.h>	// close, read, write, dup, dup2, access, execve, fork, pipe, unlink
#include <fcntl.h>	// open
#include <stdlib.h>	// exit, malloc, free
#include <stdio.h>	// perror
#include <string.h>	// strerror
//#include <sys/wait.h> // waitpid, wait
#include "../libft/libft.h"
#define ERR_USER 0
#define ERR_SYS 1

typedef struct	s_px {
	int		in;
	int		out;
	int		fd[2];
	char	**argv;
	char	**envp;
	char	*path;
}	t_px;

//utils
void	ft_error(t_px *px, char *err_msg, int err);
void	ft_free(t_px *px);
int		ft_file_open(t_px *px, int idx);


#endif