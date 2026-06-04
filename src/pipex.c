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

void	first_child(t_px *px)
{
	int	file1_fd;
	int pipe_fd[2];
	pid_t pid;

	if (pipe(pipe_fd) == -1)
		ft_error(px, "Pipe creation failed", ERR_SYS);
	pid = fork();
	if (pid < 0)
		ft_error(px, "Fork failed", ERR_SYS);
	else if (pid == 0)
	{
		// Child process here
		close(pipe_fd[0]);
	}
}