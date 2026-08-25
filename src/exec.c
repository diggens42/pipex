/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 14:43:14 by fwahl             #+#    #+#             */
/*   Updated: 2026/08/25 14:43:14 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

static char *ft_find_path(t_px *px, char *cmd)
{
	
}

void	ft_exec_cmd(t_px *px, char *cmd)
{
	px->args = ft_split(cmd, ' ');
	if (!px->args)
		ft_error(px,"malloc failed in ft_exec_cmd", ERR_SYS);
	if (!px->args[0])
		ft_error(px, cmd, 127);
	px->path = ft_find_path(px, px->args[0]);
	if (!px->path || access(px->path, F_OK) != 0)
		ft_error_cmd(px, px->args[0], 127);
	if (access(px->path, X_OK) != 0)
		ft_error_cmd(px, px->args[0], 126);
	execve(px->path, px->args, px->envp);
	ft_error(px, px->args[0], ERR_SYS);
}