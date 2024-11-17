/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szeroual <szeroual@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-11-16 19:43:28 by szeroual          #+#    #+#             */
/*   Updated: 2024-11-16 19:43:28 by szeroual         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	check_builtin(t_cmd *cmd)
{
	if (ft_strcmp(cmd->arguments[0], "cd") == 0)
		return (1);
	else if (ft_strcmp(cmd->arguments[0], "echo") == 0)
		return (2);
	else if (ft_strcmp(cmd->arguments[0], "env") == 0)
		return (3);
	else if (ft_strcmp(cmd->arguments[0], "exit") == 0)
		return (4);
	else if (ft_strcmp(cmd->arguments[0], "export") == 0)
		return (5);
	else if (ft_strcmp(cmd->arguments[0], "pwd") == 0)
		return (6);
	return (-1);
}

void	exec_builtin(int btn, t_cmd *cmd, t_mini *box)
{
	if (btn == 1 && !g_var->red_error)
		ft_cd(cmd->arguments, box->env);
	else if (btn == 2 && !g_var->red_error)
		ft_echo(cmd->arguments);
	else if (btn == 3 && !g_var->red_error)
		ft_env(box->env);
	else if (btn == 4 && !g_var->red_error)
		ft_exit(box->shell);
	else if (btn == 5 && !g_var->red_error)
		ft_export(cmd->arguments, box->env);
	else if (btn == 6 && !g_var->red_error)
		ft_pwd(box->env);
	if (g_var->out_fd > 2)
		close(g_var->out_fd);
	g_var->out_fd = 1;
}