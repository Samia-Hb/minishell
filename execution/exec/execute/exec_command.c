/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanaa <sanaa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 13:35:43 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/06 11:26:21 by sanaa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

extern char	**environ;

char	**fill_up_envi(t_envi *env, char **the_env)
{
	int		i;
	size_t	name_len;
	size_t	value_len;

	i = 0;
	while (env)
	{
		name_len = ft_strlen(env->name);
		value_len = ft_strlen(env->vale);
		the_env[i] = ft_malloc(name_len + value_len + 2, sizeof(char));
		if (!the_env[i])
		{
			perror("Error allocating memory for environment variable string");
			return (NULL);
		}
		ft_strcpy(the_env[i], env->name);
		the_env[i][name_len] = '=';
		ft_strcpy(the_env[i] + name_len + 1, env->vale);
		env = env->next;
		i++;
	}
	the_env[i] = NULL;
	return (the_env);
}

char	**separate_env(t_envi *env)
{
	int		count;
	t_envi	*tmp;
	char	**the_env;

	count = 0;
	tmp = env;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	the_env = ft_malloc((count + 1), sizeof(char *));
	if (!the_env)
	{
		perror("Error allocating memory");
		return (NULL);
	}
	return (fill_up_envi(env, the_env));
}

void	execs(t_cmd *token, int btn, t_envi *env)
{
	if (btn != -1)
	{
		exec_builtin(btn, token, env);
		clean_gc();
		rl_clear_history();
		exit(0);
	}
	else if (token->cmd_path)
	{
		g_var->en = separate_env(env);
		if (execve(token->cmd_path, token->arguments, g_var->en) == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd("\n", 2);
			exit(g_var->exit_status);
		}
	}
	else
		exit(0);
}

void	exec_builtin(int btn, t_cmd *cmd, t_envi *envi)
{
	if (btn == 1)
		ft_cd(cmd->arguments, envi);
	else if (btn == 2)
		ft_echo(cmd->arguments);
	else if (btn == 3)
		ft_env(envi);
	else if (btn == 4)
		ft_exit(cmd->arguments);
	else if (btn == 5)
		ft_export(cmd->arguments, &envi);
	else if (btn == 6)
		ft_pwd(cmd->arguments, envi);
	else if (btn == 7)
		ft_unset(cmd->arguments, &envi);
	if (g_var->out_fd > 2)
		close(g_var->out_fd);
	g_var->out_fd = 1;
}

void initialize_execution(t_cmd *token)
{
    if (!token)
        return;
    g_var->size = count_commands(token);
    g_var->pipe_nb = g_var->size - 1;
    g_var->pre_pipe_infd = -1;
    g_var->pid_array = ft_calloc(g_var->pipe_nb, sizeof(int));
}

void execute_pipes_loop(t_cmd *token)
{
    int i = 0;
    t_cmd *current = token;

    while (current)
    {
        execute_pipes(current, i, g_var->envp);
        current = current->next;
        i++;
    }
}

void handle_parent_process(t_cmd *token)
{
    int i = 0;
    t_cmd *current = token;

    while (current)
    {
        g_var->last_child_id = g_var->pid_array[i];
        parent_process();
        current = current->next;
        i++;
    }
}

void close_file_descriptors()
{
    if (g_var->pre_pipe_infd > 2)
        close(g_var->pre_pipe_infd);
}

void execute_arguments(t_cmd *token, t_envi *env)
{
    (void)env;
    if (!token)
        return;

    initialize_execution(token);
    execute_pipes_loop(token);
    handle_parent_process(token);
    close_file_descriptors();
}
