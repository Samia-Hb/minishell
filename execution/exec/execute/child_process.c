/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:40:47 by szeroual          #+#    #+#             */
/*   Updated: 2024/11/21 15:40:11 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

void	child_process(t_cmd *token, int btn, t_mini *env)
{
	g_var->last_child_id = fork();
	if (g_var->last_child_id == -1)
	{
		perror("");
		// free_hdfiles();
		exit(1);
	}
	if (g_var->last_child_id == 0)
	{
		if (g_var->pre_pipe_infd != -1)
			dup2(g_var->pre_pipe_infd, STDIN_FILENO);
		else if (token->pipe_fd[1] > 2)
			// printf("pipe_fd %d\n", token->pipe_fd[1]);
			// exit(1);
			// printf("its echo\n");
			// exit(1);
			// printf("check\n");
			dup2(token->pipe_fd[1], STDIN_FILENO);
			// close(token->pipe_fd[0]);
		if (g_var->pre_pipe_infd > 2)
			close(g_var->pre_pipe_infd);
		if (token->pipe_fd[0] > 2)
			close(token->pipe_fd[0]);
		if (token->pipe_fd[1] > 2)
			close(token->pipe_fd[1]);
		handle_file_redirections(token, btn);
		execs(token, btn, env);
	}
	else if (g_var->last_child_id > 0)
		token->pid = g_var->last_child_id;
}

void	handle_file_redirections(t_cmd *cmd, int btn)
{
	files_redirections(cmd, btn != -1);
	if (btn == -1)
		validate_cmd(cmd);
	else if (g_var->pre_pipe_infd != -1 && !cmd->file->type)
		dup2(g_var->pre_pipe_infd, STDIN_FILENO);
}

// void	child(t_cmd *cmd, int pipe_nb, int btn, t_mini *box)
// {
// 	if (g_var->last_child_id == 0)
// 	{
// 		if (g_var->pre_pipe_infd != -1)
// 			dup2(g_var->pre_pipe_infd, STDIN_FILENO);
// 		if (pipe_nb < g_var->size - 1 && cmd->pipe_fd[1] > 2)
// 			dup2(cmd->pipe_fd[1], STDOUT_FILENO);
// 		handle_file_redirections(cmd, btn);
// 		execs(cmd, btn, box);
// 		exit(0);
// 	}
// }
