/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanaa <sanaa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 10:19:37 by szeroual          #+#    #+#             */
/*   Updated: 2024/12/19 23:35:01 by sanaa            ###   ########.fr       */
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
		if (g_var->exit_status == 1)
		{
			close_open_file();
			exit(1);
		}
		execs(token, btn, g_var->envp);
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

int count_env_with_value(t_envi *env)
{
    int count = 0;
    t_envi *current = env;

    while (current)
    {
        if (current->vale != NULL)
            count++;
        current = current->next;
    }
    return count;
}

char **separate_env(t_envi *env)
{
    int count;
    char **env_array;
    int i;
    t_envi *current;

    count = count_env_with_value(env);
    env_array = (char **)malloc(sizeof(char *) * (count + 1));
    if (!env_array)
        return (NULL);
    i = 0;
    current = env;
    while (current)
    {
        if (current->vale != NULL)
        {
            env_array[i] = create_env_string(current);
            if (!env_array[i])
            {
                while (i > 0)
                    free(env_array[--i]);
                free(env_array);
                return (NULL);
            }
            i++;
        }
        current = current->next;
    }
    env_array[i] = NULL;
    return (env_array);
}
