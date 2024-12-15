/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 13:35:43 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/15 03:36:47 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

extern char	**environ;

void	execs(t_cmd *token, int btn, t_envi *env)
{
	int	exit_status;

	if (btn != -1)
	{
		exec_builtin(btn, token, env);
		close_open_file();
		rl_clear_history();
		exit(0);
	}
	else if (token->cmd_path)
	{
		g_var->en = separate_env(env);
		if (execve(token->cmd_path, token->arguments, g_var->en) == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(token->arguments[0]);
			exit(g_var->exit_status);
		}
	}
	else
	{
		exit_status = g_var->exit_status;
		close_open_file();
		exit(exit_status);
	}
}

void	initialize_execution(t_cmd *token)
{
	if (!token)
		return ;
	g_var->size = count_commands(token);
	g_var->pipe_nb = g_var->size - 1;
	g_var->pre_pipe_infd = -1;
	g_var->pid_array = ft_malloc(g_var->size, sizeof(int));
}

void	execute_pipes_loop(t_cmd *token)
{
	int		i;
	t_cmd	*current;

	i = 0;
	current = token;
	while (current)
	{
		execute_pipes(current, i, g_var->envp);
		current = current->next;
		i++;
	}
}

void	handle_parent_process(t_cmd *token)
{
	int		i;
	t_cmd	*current;

	i = 0;
	current = token;
	while (current && i < g_var->size)
	{
		g_var->last_child_id = g_var->pid_array[i];
		parent_process();
		current = current->next;
		i++;
	}
}

void	execute_arguments(t_cmd *token, t_envi *env)
{
	(void)env;
	if (!token)
		return ;
	initialize_execution(token);
	execute_pipes_loop(token);
	handle_parent_process(token);
	close_file_descriptors();
}
