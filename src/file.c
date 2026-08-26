/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 21:00:57 by fwahl             #+#    #+#             */
/*   Updated: 2026/08/25 21:00:57 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

static char *get_env_path(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp || !envp[i])
		return (NULL);
	return (envp[i] + 5);
}

static char	*join_path(char *dir, char *cmd)
{
	char *temp;
	char *full_path;

	temp = ft_strjoin(dir, "/");
	if (!temp)
		return (NULL);
	full_path = ft_strjoin(temp, cmd);
	free(temp);
	return (full_path);
}

char	*find_path(t_px *px)
{
	char	**dir_paths;
	char	*full_path;
	int		i;

	if (ft_strchr(px->args[0], '/') || !get_env_path(px->envp))
		return (ft_strdup(px->args[0]));
	dir_paths = ft_split(get_env_path(px->envp), ':');
	if (!dir_paths)
		ft_error(px, "malloc failed in ft_find_path", ERR_SYS);
	full_path = NULL;
	i = -1;
	while (dir_paths[++i])
	{
		full_path = join_path(dir_paths[i], px->args[0]);
		if (full_path && access(full_path, X_OK) == 0)
			break ;
		free(full_path);
		full_path = NULL;
	}
	return (ft_free_strarray(dir_paths), full_path);
}

static int	file_open(t_px *px, int idx)
{
	int file_fd;

	if (idx == 1)
		file_fd = open(px->argv[1], O_RDONLY);
	if (idx == 4)
		file_fd = open(px->argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (file_fd < 0)
		ft_error(px, "File opening failed on index " + idx, ERR_SYS);
	return (file_fd);
}

void init_file_fds(t_px *px)
{
	px->in = file_open(px, 1);
	px->out = file_open(px, 4);
	if (pipe(px->fd) == -1)
		ft_error(px, "Pipe creation failed", ERR_SYS);
}