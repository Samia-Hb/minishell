/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 00:02:59 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/01 20:59:24 by shebaz           ###   ########.fr       */
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

void	red_builtin(t_cmd *token, int btn, t_mini *box)
{
	files_redirections(token, 1);
	exec_builtin(btn, token, box);
}

void	close_files(t_cmd *token)
{
	if (token->pipe_fd[1] > 2)
		close(token->pipe_fd[1]);
	if (g_var->pre_pipe_infd > 2)
		close(g_var->pre_pipe_infd);
}

void	execute_pipes(t_cmd *token, int pipe_nb, t_mini *env)
{
	int	original_stdin;
	int	original_stdout;
	int status;
	int	btn;

	original_stdin = dup(STDIN_FILENO);
	original_stdout = dup(STDOUT_FILENO);
	btn = check_builtin(token);
	if (g_var->size == 1 && btn != -1)
	{
		red_builtin(token, btn, env);
		dup2(original_stdin, STDIN_FILENO);
		dup2(original_stdout, STDOUT_FILENO);
	}
	else
	{
		if (g_var->size != pipe_nb + 1)
		{
			if (pipe(token->pipe_fd) == -1)
				printf("Pipeline error\n");
		}
		child_process(token, pipe_nb, btn, env);
		close_files(token);
		g_var->pre_pipe_infd = token->pipe_fd[0];
		if (g_var->last_child_id > 0)
		{
			waitpid(g_var->last_child_id, &status, 0);
			if (WIFSIGNALED(status))
			{
				printf("STAAAAT = %d\n",WEXITSTATUS(status));
				if (WEXITSTATUS(status) == 2)
				{
					printf("abaaa oui\n");
				}
				g_var->exit_status = WEXITSTATUS(status);
				// printf("exiiiiit = %d\n", g_var->exit_status);
				// // printf("check yees\n");
			}
			g_var->exit_status = WEXITSTATUS(status);
		}
	}
	close(original_stdin);
	close(original_stdout);
}
