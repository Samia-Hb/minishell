/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:40:47 by szeroual          #+#    #+#             */
/*   Updated: 2024/12/01 18:30:12 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

void sig(int sig)
{
	if (sig == SIGINT)
	{
		printf ("\n");
		g_var->exit_status = 130;
	}
	else if (sig == SIGTERM)
	{
		printf("^\\Quit (core dumped)\n");
		g_var->exit_status = 131;
	}
}

void handle_child_signal()
{
	signal(SIGINT, sig);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTERM, SIG_DFL);
}

void	child_process(t_cmd *token, int pipe_nb, int btn, t_mini *env)
{
	(void)pipe_nb;
	g_var->last_child_id = fork();
	handle_child_signal();
	if (g_var->last_child_id == 0)
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
		// write(2, "yes\n", 4);
		execs(token, btn, env);
		exit(g_var->exit_status);
	}
}
void	handle_file_redirections(t_cmd *cmd, int btn)
{
	files_redirections(cmd, btn != -1);
	if (btn == -1)
		validate_cmd(cmd);
}
