/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 00:02:59 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/01 11:49:10 by shebaz           ###   ########.fr       */
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

void	in_file_prep(char *path, int is_builtin)
{
	int	fd;

	fd = open(path, O_RDONLY);
	if (fd == -1)
	{
		g_var->exit_status = 1;
		g_var->red_error = 1;
		ft_putstr_fd("minishell: ", 2);
		perror(path);
		if (!is_builtin || g_var->size > 1)
			exit(1);
	}
	else
	{
		if (dup2(fd, 0) == -1)
		{
			perror("dup2");
			exit(1);
		}
		if (fd > 2)
			close(fd);
	}
}

void	out_file_prep(char *path, int is_builtin)
{
	int	fd;

	fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (fd == -1)
	{
		g_var->exit_status = 1;
		g_var->red_error = 1;
		ft_putstr_fd("minishell: ", 2);
		perror(path);
		if (!is_builtin || g_var->size > 1)
			exit(1);
	}
	else
	{
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit(1);
		}
		if (fd > 2)
			close(fd);
	}
}

void	append_file_prep(t_cmd *token, char *path, int is_builtin)
{
	int	fd;

	(void)token;
	fd = open(path, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd == -1)
	{
		g_var->exit_status = 1;
		g_var->red_error = 1;
		ft_putstr_fd("minishell: ", 2);
		perror(path);
		if (!is_builtin || g_var->size > 1)
			exit(1);
	}
	else
	{
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			close(fd);
			exit(1);
		}
		if (fd > 2)
			close(fd);
	}
}

void	append_heredoc_prep(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY, 0777);
	if (fd == -1)
	{
		write(2, "Error up here\n", 15);
		exit(g_var->exit_status);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	unlink(filename);
}

void	files_redirections(t_cmd *cmd, int builtin)
{
	t_file	*curr_red;

	g_var->size = count_commands(cmd);
	curr_red = cmd->file;
	while (curr_red)
	{
		if (curr_red->type == 1)
			out_file_prep(curr_red->filename, builtin);
		else if (curr_red->type == 2)
			in_file_prep(curr_red->filename, builtin);
		else if (curr_red->type == 3)
			append_heredoc_prep(curr_red->filename);
		else if (curr_red->type == 4)
			append_file_prep(cmd, curr_red->filename, builtin);
		curr_red = curr_red->next;
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
	int	btn;
	int	original_stdin;
	int	original_stdout;

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
			waitpid(g_var->last_child_id, NULL, 0);
	}
	close(original_stdin);
	close(original_stdout);
}
