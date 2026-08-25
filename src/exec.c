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

static char	*ft_join_path(char *dir, char *cmd)
{
	
}

static char	*ft_find_path(t_px *px, char *cmd)
{
	char	**dir_paths;
	char	*full_path;
	int		i;

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	dir_paths = ft_split(ft_get_env_path(px->envp), ':');
	if (!dir_paths)
		ft_error(px, "malloc failed in ft_find_path", ERR_SYS);
	i = 0;
	while (dir_paths[i])
	{
		full_path = ft_strjoin(dir_paths[i], "/");
		if (full_path && access(full_path, X_OK) == 0)
			break ;
		free(full_path);
		full_path = NULL;
		i++;
	}
	ft_free_strarray(dir_paths);
	return (NULL);
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