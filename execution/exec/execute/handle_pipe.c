/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 00:02:59 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/15 02:35:50 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

void	red_builtin(t_cmd *token, int btn, t_envi *envi)
{
	files_redirections(token, 1);
	if (g_var->exit_status == 1)
		return ;
	exec_builtin(btn, token, envi);
}

void	close_files(t_cmd *token)
{
	if (token->pipe_fd[1] > 2)
		close(token->pipe_fd[1]);
	if (g_var->pre_pipe_infd > 2)
		close(g_var->pre_pipe_infd);
}

void	parent_process(void)
{
	int	status;

	status = 0;
	waitpid(g_var->last_child_id, &status, 0);
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == 2)
			g_var->exit_status = 130;
		else if (WTERMSIG(status) == 3)
			g_var->exit_status = 131;
	}
	else if (WEXITSTATUS(status))
	{
		g_var->exit_status = WEXITSTATUS(status);
		if (status == 256)
			g_var->exit_status = 1;
	}
}

void	save_retrive(int original_stdin, int original_stdout, int nbr)
{
	if (nbr == 1)
	{
		close(original_stdin);
		close(original_stdout);
	}
	else if (nbr == 2)
	{
		dup2(original_stdin, STDIN_FILENO);
		dup2(original_stdout, STDOUT_FILENO);
	}
}

void	execute_pipes(t_cmd *token, int pipe_nb, t_envi *env)
{
	int	original_stdin;
	int	original_stdout;

	original_stdin = dup(STDIN_FILENO);
	original_stdout = dup(STDOUT_FILENO);
	g_var->cpy_in_fd = original_stdin;
	g_var->cpy_out_fd = original_stdout;
	if (g_var->size == 1 && check_builtin(token) != -1)
	{
		red_builtin(token, check_builtin(token), env);
		save_retrive(original_stdin, original_stdin, 2);
	}
	else
	{
		if (g_var->size != pipe_nb + 1)
		{
			if (pipe(token->pipe_fd) == -1)
				write(2, "Error\n", 6);
		}
		child_process(token, check_builtin(token));
		close_files(token);
		g_var->pre_pipe_infd = token->pipe_fd[0];
	}
	close(STDERR_FILENO);
	save_retrive(original_stdin, original_stdout, 1);
}
