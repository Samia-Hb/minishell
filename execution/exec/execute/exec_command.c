/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:41:37 by szeroual          #+#    #+#             */
/*   Updated: 2024/11/23 18:12:23 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

extern char **environ;

char **separate_env(t_envi *env)
{
    int count = 0;
    t_envi *tmp = env;
    while (tmp)
    {
        count++;
        tmp = tmp->next;
    }
    char **the_env = malloc(sizeof(char *) * (count + 1));
    if (!the_env)
    {
        perror("Error allocating memory");
        return NULL;
    }
    int i = 0;
    while (env)
    {
        size_t name_len = strlen(env->name);
        size_t value_len = strlen(env->vale);
        the_env[i] = malloc(name_len + value_len + 2);
        if (!the_env[i])
        {
            perror("Error allocating memory for environment variable string");
            return NULL;
        }
        strcpy(the_env[i], env->name);
        the_env[i][name_len] = '=';
        strcpy(the_env[i] + name_len + 1, env->vale);
        env = env->next;
        i++;
    }
    the_env[i] = NULL;
    return the_env;
}


void	execs(t_cmd *cmd, int btn, t_mini *box)
{
    char **env_array;

    if (btn != -1)
    {
        exec_builtin(btn, cmd, box);
        exit(0);
    }
    if (cmd->cmd_path)
    {
        env_array = separate_env(box->env); 
        if (!env_array)
        {
            perror("Failed to convert environment variables");
            exit(1);
        }
        execve(cmd->cmd_path, cmd->arguments, env_array);
        perror(cmd->cmd_path);
        free(env_array); 
        exit(errno);
    }
}

// void	execute_arguments(t_cmd *cmd, t_mini *box)
// {
// 	int	i;

// 	g_var->size = count_commands(cmd);
// 	g_var->pipe_nb = g_var->size - 1;
// 	g_var->exit_status = 0;
// 	g_var->pre_pipe_infd = -1;
// 	i = 0;
// 	while (cmd && g_var->exit_status == 0)
// 	{
// 		execute_pipes(cmd, i, box);
// 		cmd = cmd->next;
// 		i++;
// 	}
// 	if (g_var->pre_pipe_infd > 2)
// 		close(g_var->pre_pipe_infd);
// 	sig_wait(cmd);
// }
