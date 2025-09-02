/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 15:51:11 by szeroual          #+#    #+#             */
/*   Updated: 2024/12/21 05:53:37 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

void	sig(int sig)
{
	if (sig == SIGINT)
		g_var->exit_status = 130;
	if (sig == SIGQUIT)
	{
		printf("Quit (core dumped)\n");
		g_var->exit_status = 131;
	}
}

char	**allocate_env_array(int count)
{
	char	**env_array;

	env_array = (char **)malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	return (env_array);
}

int	fill_env_array(t_envi *env, char **env_array)
{
	int		i;
	t_envi	*current;

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
				return (-1);
			}
			i++;
		}
		current = current->next;
	}
	env_array[i] = NULL;
	return (0);
}

char	**create_env_array(t_envi *env, int count)
{
	char	**env_array;

	env_array = allocate_env_array(count);
	if (!env_array)
		return (NULL);
	if (fill_env_array(env, env_array) == -1)
		return (NULL);
	return (env_array);
}

void	setup_signals_and_fork(void)
{
	g_var->last_child_id = fork();
	signal(SIGINT, sig);
	signal(SIGQUIT, SIG_IGN);
}
