/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 15:16:10 by fwahl             #+#    #+#             */
/*   Updated: 2026/06/04 15:16:10 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

static void	ft_exec_cmd(t_px *px, char *cmd)
{
	px->args = ft_split(cmd, ' ');
	if (!px->args)
		ft_error(px,"malloc failed in ft_exec_cmd", ERR_SYS);
	if (!px->args[0])
		ft_error(px, cmd, 127);
	px->path = ft_find_path(px);
	if (!px->path || access(px->path, F_OK) != 0)
		ft_error_cmd(px, px->args[0], 127);
	if (access(px->path, X_OK) != 0)
		ft_error_cmd(px, px->args[0], 126);
	execve(px->path, px->args, px->envp);
	ft_error(px, px->args[0], ERR_SYS);
}

static void	ft_child_io(t_px *px, int idx)
{
	if (idx == 0)
	{
		if (dup2(px->in, STDIN_FILENO) == -1
			|| dup2(px->fd[1], STDOUT_FILENO) == -1)
			ft_error(px, "dup2", ERR_SYS);
	}
	else
	{
		if (dup2(px->out, STDOUT_FILENO) == -1)
			ft_error(px, "dup2", ERR_SYS);
	}
	ft_free(px);
}

void	ft_child(t_px *px, int idx)
{
	px->pid[idx] = fork();
	if (px->pid[idx] == -1)
		ft_error(px, "Fork failed on index " + idx, ERR_SYS);
	if (px->pid[idx] == 0)
	{
		ft_child_io(px, idx);
		ft_exec_cmd(px, px->argv[idx + 2]);
	}
}