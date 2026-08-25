/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 12:21:26 by fwahl             #+#    #+#             */
/*   Updated: 2026/06/04 12:21:26 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

static void init_file_fds(t_px *px)
{
	px->in = ft_file_open(px, 1);
	px->out = ft_file_open(px, 4);
	if (pipe(px->fd) == -1)
		ft_error(px, "Pipe creation failed", ERR_SYS);
}

static t_px init_px(char **argv, char **envp)
{
	t_px	px;

	px.in = -1;
	px.out = -1;
	px.fd[0] = -1;
	px.fd[1] = -1;
	px.pid[0] = -1;
	px.pid[1] = -1;
	px.argv = argv;
	px.envp = envp;
	px.path = NULL;
	px.args = NULL;
	return (px);
}

int main(int argc, char **argv, char **envp)
{
	t_px	px;

	px = init_px(argv, envp);
	if (argc != 5)
		ft_error(&px, "Usage: ./pipex file1 cmd1 cmd2 file2\n", ERR_USER);
	init_file_fds(&px);
	

	return (0);
}