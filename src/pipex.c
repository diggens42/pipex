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

int	wait_childs(t_px *px)
{
	int	temp_status;
	int	status;

	temp_status = 0;
	status = 0;
	waitpid(px->pid[0], NULL, 0);
	waitpid(px->pid[1], &temp_status, 0);
	if (WIFEXITED(temp_status))
		status = WEXITSTATUS(temp_status);
	else if (WIFSIGNALED(temp_status))
		status = 128 + WTERMSIG(temp_status);
	return (status);
}

static void	ft_exec_cmd(t_px *px)
{
	px->args = ft_split(px->argv[px->idx +2], ' ');
	if (!px->args)
		ft_error(px, "malloc failed in ft_exec_cmd", ERR_SYS);
	if (!px->args[0])
		ft_error_cmd(px, px->argv[px->idx + 2], 127);
	px->path = find_path(px);
	if (!px->path || access(px->path, F_OK) != 0)
		ft_error_cmd(px, px->args[0], 127);
	if (access(px->path, X_OK) != 0)
		ft_error_cmd(px, px->args[0], 126);
	execve(px->path, px->args, px->envp);
	ft_error(px, px->args[0], ERR_SYS);
}

static void	ft_child_io(t_px *px)
{
	if (px->idx == 0)
	{
		px->in = file_open(px, 1);
		if (dup2(px->in, STDIN_FILENO) == -1
			|| dup2(px->fd[1], STDOUT_FILENO) == -1)
			ft_error(px, "dup2", ERR_SYS);
	}
	else
	{
		px->out = file_open(px, 4);
		if (dup2(px->fd[0], STDIN_FILENO) == -1
			|| dup2(px->out, STDOUT_FILENO) == -1)
			ft_error(px, "dup2", ERR_SYS);
	}
	ft_free(px);
}

void	ft_pipex(t_px *px, int idx)
{
	px->idx = idx;
	px->pid[idx] = fork();
	if (px->pid[idx] == -1)
		ft_error(px, "fork", ERR_SYS);
	if (px->pid[idx] == 0)
	{
		ft_child_io(px);
		ft_exec_cmd(px);
	}
}
