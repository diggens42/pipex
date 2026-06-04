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
