/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanaa <sanaa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 15:51:11 by szeroual          #+#    #+#             */
/*   Updated: 2024/12/21 03:31:31 by sanaa            ###   ########.fr       */
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

void	handle_file_redirections(t_cmd *cmd, int btn)
{
	files_redirections(cmd, btn != -1);
	if (btn == -1)
		validate_cmd(cmd);
}

char **allocate_env_array(int count)
{
    char **env_array = (char **)malloc(sizeof(char *) * (count + 1));
    if (!env_array)
        return NULL;
    return env_array;
}

int fill_env_array(t_envi *env, char **env_array)
{
    int i = 0;
    t_envi *current = env;

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
                return -1;
            }
            i++;
        }
        current = current->next;
    }
    env_array[i] = NULL;
    return 0;
}

char **create_env_array(t_envi *env, int count)
{
    char **env_array = allocate_env_array(count);
    if (!env_array)
        return NULL;
    if (fill_env_array(env, env_array) == -1)
        return NULL;
    return env_array;
}

char **separate_env(t_envi *env)
{
    int count = count_env_with_value(env);
    return create_env_array(env, count);
}

void setup_signals_and_fork()
{
    g_var->last_child_id = fork();
    signal(SIGINT, sig);
    signal(SIGQUIT, sig);
}
