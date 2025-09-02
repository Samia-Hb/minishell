/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 03:30:29 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/21 05:26:07 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	check_heredoc_sign(int status, char *filename, int fd)
{
	(void)fd;
	if (WEXITSTATUS(status))
	{
		unlink(filename);
		g_var->stop = 1;
	}
}
