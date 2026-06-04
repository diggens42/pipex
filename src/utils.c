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

void	ft_error(char *err_msg, int err)
{
	if (err == ERR_USER)
	{
		ft_putstr_fd(err_msg, ERR_USER);
		exit(EXIT_FAILURE);
	}
}
