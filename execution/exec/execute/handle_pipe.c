/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 00:02:59 by shebaz            #+#    #+#             */
/*   Updated: 2024/11/26 18:22:33 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

void	handle_file_descriptors(t_cmd *token)
{
	if (token->pipe_fd[1] > 2)
		close(token->pipe_fd[1]);
	if (g_var->pre_pipe_infd > 2)
		close(g_var->pre_pipe_infd);
	g_var->pre_pipe_infd = token->pipe_fd[0];
}

void	handle_pipe_creation(t_cmd *token, int pipe_nb)
{
	if (g_var->size != pipe_nb + 1)
	{
		if (pipe(token->pipe_fd) == -1)
		{
			perror("pipe");
			exit(1);
		}
	}
	else
	{
		token->pipe_fd[0] = 0;
		token->pipe_fd[1] = 0;
	}
}

void	execute_pipes(t_cmd *token, int pipe_nb, t_mini *env)
{
	
	token->builtin = check_builtin(token);
	g_var->size = count_commands(token);

	if (g_var->size == 1 && token->builtin != -1)
	{
		files_redirections(token, 1);
		exec_builtin(token->builtin, token, env);
	}
	else
	{
		handle_pipe_creation(token, pipe_nb);
		child_process(token, token->builtin, env);
		handle_file_descriptors(token);
	}
}