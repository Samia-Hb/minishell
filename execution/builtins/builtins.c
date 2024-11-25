/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 09:50:00 by shebaz            #+#    #+#             */
/*   Updated: 2024/11/25 19:52:33 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// int	check_builtin(t_cmd *cmd)
// {
// 	if (!cmd->arguments || !cmd->arguments[0])
// 		return (-1);
// 	if (!ft_strcmp(cmd->arguments[0], "cd"))
// 		return (1);
// 	else if (!ft_strcmp(cmd->arguments[0], "echo"))
// 		return (2);
// 	else if (!ft_strcmp(cmd->arguments[0], "env"))
// 		return (3);
// 	else if (!ft_strcmp(cmd->arguments[0], "exit"))
// 		return (4);
// 	else if (!ft_strcmp(cmd->arguments[0], "export"))
// 		return (5);
// 	else if (!ft_strcmp(cmd->arguments[0], "pwd"))
// 		return (6);
// 	else if (!ft_strcmp(cmd->arguments[0], "unset"))
// 		return (7);
// 	return (-1);
// }

// void	exec_builtin(int btn, t_cmd *cmd, t_mini *box)
// {
// 	if (btn == 1 && !g_var->red_error)
// 		ft_cd(cmd->arguments, box->env);
// 	else if (btn == 2 && !g_var->red_error)
// 		ft_echo(cmd->arguments);
// 	else if (btn == 3 && !g_var->red_error)
// 		ft_env(box->env);
// 	else if (btn == 4 && !g_var->red_error)
// 		ft_exit(cmd->arguments);
// 	else if (btn == 5 && !g_var->red_error)
// 		ft_export(cmd->arguments, &box->env);
// 	else if (btn == 6 && !g_var->red_error)
// 		ft_pwd(cmd->arguments, box->env);
// 	else if (btn == 7 && !g_var->red_error)
// 		ft_unset(cmd->arguments, box);
// 	if (g_var->out_fd > 2)
// 		close(g_var->out_fd);
// 	g_var->out_fd = 1;
// }
