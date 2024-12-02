/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 13:35:43 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/01 23:00:16 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

extern char	**environ;

char	**separate_env(t_envi *env)
{
	int		count;
	t_envi	*tmp;
	char	**the_env;
	int		i;
	size_t	name_len;
	size_t	value_len;

	count = 0;
	tmp = env;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	the_env = malloc(sizeof(char *) * (count + 1));
	if (!the_env)
	{
		perror("Error allocating memory");
		return (NULL);
	}
	i = 0;
	while (env)
	{
		name_len = strlen(env->name);
		value_len = strlen(env->vale);
		the_env[i] = malloc(name_len + value_len + 2);
		if (!the_env[i])
		{
			perror("Error allocating memory for environment variable string");
			return (NULL);
		}
		strcpy(the_env[i], env->name);
		the_env[i][name_len] = '=';
		strcpy(the_env[i] + name_len + 1, env->vale);
		env = env->next;
		i++;
	}
	the_env[i] = NULL;
	return (the_env);
}

void	execs(t_cmd *token, int btn, t_mini *env)
{
	if (btn != -1)
	{
		exec_builtin(btn, token, env);
		exit(g_var->exit_status);
	}
	if (token->cmd_path)
	{
		// write(2, "yes\n", 4);
		g_var->en = separate_env(env->env);
		if (execve(token->cmd_path, token->arguments, g_var->en) == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd("\n", 2);
			exit(g_var->exit_status);
		}
		// write(2, "uppp\n",5);
	}
	else
		exit(g_var->exit_status);
}

int	check_builtin(t_cmd *cmd)
{
	if (!cmd->arguments || !cmd->arguments[0])
		return (-1);
	if (!ft_strcmp(cmd->arguments[0], "cd"))
		return (1);
	else if (!ft_strcmp(cmd->arguments[0], "echo"))
		return (2);
	else if (!ft_strcmp(cmd->arguments[0], "env"))
		return (3);
	else if (!ft_strcmp(cmd->arguments[0], "exit"))
		return (4);
	else if (!ft_strcmp(cmd->arguments[0], "export"))
		return (5);
	else if (!ft_strcmp(cmd->arguments[0], "pwd"))
		return (6);
	else if (!ft_strcmp(cmd->arguments[0], "unset"))
		return (7);
	return (-1);
}

void	exec_builtin(int btn, t_cmd *cmd, t_mini *box)
{
	if (btn == 1)
		ft_cd(cmd->arguments, box->env);
	else if (btn == 2)
		ft_echo(cmd->arguments);
	else if (btn == 3)
		ft_env(box->env);
	else if (btn == 4)
		ft_exit(cmd->arguments);
	else if (btn == 5)
		ft_export(cmd->arguments, &box->env);
	else if (btn == 6)
		ft_pwd(cmd->arguments, box->env);
	else if (btn == 7)
		ft_unset(cmd->arguments, box);
	if (g_var->out_fd > 2)
		close(g_var->out_fd);
	g_var->out_fd = 1;
}

void	execute_arguments(t_cmd *token, t_mini *env)
{
	int		i;
	t_cmd	*current;

	if (!token)
		return ;
	i = 0;
	current = token;
	g_var->size = count_commands(token);
	g_var->pipe_nb = g_var->size - 1;
	g_var->exit_status = 0;
	g_var->pre_pipe_infd = -1;
	while (current && g_var->exit_status == 0)
	{
		execute_pipes(current, i, env);
		current = current->next;
		i++;
	}
	if (g_var->pre_pipe_infd > 2)
		close(g_var->pre_pipe_infd);
	// sig_wait(token);
}
