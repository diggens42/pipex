/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 14:52:54 by fwahl             #+#    #+#             */
/*   Updated: 2026/06/04 14:52:54 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

static void	close_fd(int *fd)
{
	if (*fd >= 0)
		close(*fd);
	*fd = -1;
}

void	ft_free(t_px *px)
{
	close_fd(&px->in);
	close_fd(&px->out);
	close_fd(&px->fd[0]);
	close_fd(&px->fd[1]);
	if (px->path)
		free(px->path);
	px->path = NULL;
	if (px->args)
		ft_free_strarray(px->args);
	px->args = NULL;
}

void	ft_error(t_px *px, char *err_msg, int err)
{
	if (err == ERR_USER)
		ft_putstr_fd(err_msg, STDERR_FILENO);
	else
		perror(err_msg);
	ft_free(px);
	exit(EXIT_FAILURE);
}

void	ft_error_cmd(t_px *px, char *cmd, int status)
{
	ft_putstr_fd("pipex: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	if (status == 127)
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
	else
		ft_putstr_fd(": permission denied\n", STDERR_FILENO);
	ft_free(px);
	exit(status);
}
