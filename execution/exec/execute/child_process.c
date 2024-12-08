/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:40:47 by szeroual          #+#    #+#             */
/*   Updated: 2024/12/06 21:45:39 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

void	sig(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		g_var->exit_status = 130;
	}
	if (sig == SIGQUIT)
	{
		printf("Quit (core dumped)\n");
		g_var->exit_status = 131;
	}
}

void	child_process(t_cmd *token, int btn)
{
	g_var->last_child_id = fork();
	signal(SIGINT, sig);
	signal(SIGQUIT, sig);
	if (!g_var->last_child_id)
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
		execs(token, btn, g_var->envp);
		clean_gc();
		exit(g_var->exit_status);
	}
}

void	handle_file_redirections(t_cmd *cmd, int btn)
{
	files_redirections(cmd, btn != -1);
	if (btn == -1)
		validate_cmd(cmd);
}

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
