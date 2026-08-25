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

int	ft_file_open(t_px *px, int idx)
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

static void	ft_child_io(t_px *px, int idx)
{
	if (idx == 0)
	{

	}
	else
	{

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
		
	}
}