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

void	ft_error(t_px *px, char *err_msg, int err)
{
	if (err == ERR_USER)
		ft_putstr_fd(err_msg, ERR_USER);
	if (err == ERR_SYS)
		perror(err_msg);
	ft_free(px);
	exit(EXIT_FAILURE);
}

void	ft_free(t_px *px)
{
	if (px->in != -1)
		close(px->in);
	if (px->out != -1)
		close(px->out);
	if (px->fd[0] != -1)
		close(px->fd[0]);
	if (px->fd[1] != -1)
		close(px->fd[1]);
}

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