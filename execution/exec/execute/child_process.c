/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 14:43:50 by szeroual          #+#    #+#             */
/*   Updated: 2024/12/21 03:46:54 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

void	handle_child_process(t_cmd *token, int btn)
{
	if (g_var->pre_pipe_infd != -1)
		dup2(g_var->pre_pipe_infd, STDIN_FILENO);
	if (token->pipe_fd[1] > 2)
		dup2(token->pipe_fd[1], STDOUT_FILENO);
	if (g_var->pre_pipe_infd > 2)
		close(g_var->pre_pipe_infd);
	if (token->pipe_fd[0] > 2)
		close(token->pipe_fd[0]);
	if (token->pipe_fd[1] > 2)
		close(token->pipe_fd[1]);
	handle_file_redirections(token, btn);
	if (g_var->exit_status == 1)
	{
		close_open_file();
		exit(1);
	}
	if (!token->arguments[0])
	{
		close_open_file();
		exit(0);
	}
	execs(token, btn, g_var->envp);
}

void	child_process(t_cmd *token, int btn)
{
	setup_signals_and_fork();
	if (!g_var->last_child_id)
		handle_child_process(token, btn);
}

char	**separate_env(t_envi *env)
{
	int	count;

	count = count_env_with_value(env);
	return (create_env_array(env, count));
}

void	handle_file_redirections(t_cmd *cmd, int btn)
{
	files_redirections(cmd, btn != -1);
	if (btn == -1)
		validate_cmd(cmd);
}
