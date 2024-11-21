/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:41:37 by szeroual          #+#    #+#             */
/*   Updated: 2024/11/21 15:35:35 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

void	execs(t_cmd *cmd, int btn, t_mini *box)
{
	if (btn != -1)
	{
		exec_builtin(btn, cmd, box);
		exit(0);
	}
	if (cmd->cmd_path)
	{
		execve(cmd->cmd_path, cmd->arguments, NULL);
		perror(cmd->cmd_path);
		exit(errno);
	}
}
