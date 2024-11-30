/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 00:02:59 by shebaz            #+#    #+#             */
/*   Updated: 2024/11/30 11:49:40 by shebaz           ###   ########.fr       */
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

int	*red_builtin(t_cmd *token, int btn, t_mini *box, int *size)
{
	int *arr;


	arr = files_redirections(token, 1, size);
	exec_builtin(btn, token, box);
	return (arr);
}
void	close_files(t_cmd *token)
{
	if (token->pipe_fd[1] > 2)
		close(token->pipe_fd[1]);
	if (g_var->pre_pipe_infd > 2)
		close(g_var->pre_pipe_infd);
}
void close_reder(int *arr)
{
	int i;

	i = 0;
	if(arr)
	{
		while (arr[i])
		{
			printf("arr[%d] == %d\n",i,arr[i]);
			close(arr[i]);
			i++;
		}
		free(arr);		
	}
}
void	execute_pipes(t_cmd *token, int pipe_nb, t_mini *env)
{
	int	original_stdin = dup(STDIN_FILENO);
	int	btn = check_builtin(token);
	int size = 0;
	if (g_var->size == 1 && btn != -1)
	{
		// printf("check here\n");
		int *arr = red_builtin(token, btn, env, &size);
		close_reder(arr);
	}
	else
	{
		if (g_var->size != pipe_nb + 1)
		{
			if (pipe(token->pipe_fd) == -1)
				perror("Error");
		}
		child_process(token, pipe_nb, btn, env);
		close_files(token);
		g_var->pre_pipe_infd = token->pipe_fd[0];
		if (g_var->last_child_id > 0)
			waitpid(g_var->last_child_id, NULL, 0);
	}
	close(original_stdin);

	// token->builtin = check_builtin(token);
	// g_var->size = count_commands(token);
	// if (g_var->size == 1 && token->builtin != -1 && !g_var->tmp)
	// {
	// 	files_redirections(token, 1);
	// 	exec_builtin(token->builtin, token, env);
	// }
	// else
	// {
	// 	g_var->tmp = 1;
	// 	handle_pipe_creation(token, pipe_nb);
	// 	child_process(token,5 ,token->builtin, env);
	// 	handle_file_descriptors(token);
	// }
}