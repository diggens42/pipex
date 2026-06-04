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

static void init_px(t_px *px, char **argv, char **envp)
{
	px->in = -1;
	px->out = -1;
	px->fd[0] = -1;
	px->fd[1] = -1;
	px->argv = argv;
	px->envp = envp;
	px->path = NULL;
}

int main(int argc, char **argv, char **envp)
{
	t_px	px;

	init_px(&px, argv, envp);
	if (argc != 5)
		ft_error(&px, "Wrong number of args!\n", ERR_USER);
	

	return (0);
}