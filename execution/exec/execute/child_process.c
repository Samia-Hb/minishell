/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:40:47 by szeroual          #+#    #+#             */
/*   Updated: 2024/11/30 13:33:04 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

void	child_process(t_cmd *token, int pipe_nb, int btn, t_mini *env)
{
	(void)pipe_nb;
	g_var->last_child_id = fork();
	if (g_var->last_child_id == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
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
		execs(token, btn, env);
		exit(0);
	}
}

void	handle_file_redirections(t_cmd *cmd, int btn)
{
	files_redirections(cmd, btn != -1);
	if (btn == -1)
		validate_cmd(cmd);
}

int	count_commands(t_cmd *cmd)
{
	int		count;
	t_cmd	*current;

	count = 0;
	current = cmd;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

char	*put_cmd_status(int status, char *cmd_path, char *cmd)
{
	if (status)
	{
		if (status == 1 && cmd && cmd[0] != '$')
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd, 2);
			ft_putstr_fd(": command not found\n", 2);
			exit(127);
		}
		else if (cmd && cmd[0] != '$')
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd, 2);
			ft_putstr_fd(": permission denied\n", 2);
			exit(126);
		}
		return (NULL);
	}
	else
		return (cmd_path);
}

int	handle_stat_error(char *path, int is_builtin)
{
	ft_putstr_fd("minishell: ", 2);
	perror(path);
	if (is_builtin)
	{
		g_var->red_error = 1;
		g_var->exit_status = 1;
		return (0);
	}
	else
	{
		exit(1);
	}
}
