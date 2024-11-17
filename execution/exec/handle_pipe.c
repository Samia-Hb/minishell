/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szeroual <szeroual@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-11-16 13:07:29 by szeroual          #+#    #+#             */
/*   Updated: 2024-11-16 13:07:29 by szeroual         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	execute_pipes(t_cmd *token, int pipe_nb, t_mini *env)
{
	int btn;
	int original_stdin;

	original_stdin = dup(STDIN_FILENO);
	btn = check_builtin(token);
	if (g_var->size == 1 && btn != -1)
	{
		files_redirections(token, 1);
		exec_builtin(btn, token, env);
	}
	else
	{
		if (g_var->size != pipe_nb + 1)
		{
			if (pipe(token->pipe_fd) == -1)
			{
				perror("pipe");
				exit(1);
			}
		}
		child_process(token, pipe_nb, btn, env);
		if (token->pipe_fd[1] > 2)
			close(token->pipe_fd[1]);
		if (g_var->pre_pipe_infd > 2)
			close(g_var->pre_pipe_infd);
		g_var->pre_pipe_infd = token->pipe_fd[0];
		if (g_var->last_child_id > 0)
			waitpid(g_var->last_child_id, NULL, 0);
	}
	close(original_stdin);
}