/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szeroual <szeroual@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 10:19:08 by szeroual          #+#    #+#             */
/*   Updated: 2024/12/18 10:19:11 by szeroual         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

char	**env_to_array(t_envi *env)
{
	int		count;
	char	**env_array;
	int		i;

	count = count_valid_env_entries(env);
	env_array = (char **)malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	if (!populate_env_array(env_array, env, count))
	{
		i = 0;
		while (env_array[i])
			free(env_array[i++]);
		free(env_array);
		return (NULL);
	}
	return (env_array);
}

void	sync_env_array(t_envi *env)
{
	char	**new_env;

	new_env = env_to_array(env);
	if (!new_env)
		return ;
	free_env_array(g_var->en);
	g_var->en = new_env;
}

static void	init_copy_env(t_envi **new_env, t_envi **last_node)
{
	*new_env = NULL;
	*last_node = NULL;
}

t_envi	*process_env_node(t_envi *current, t_envi **last_node)
{
	t_envi	*new_node;
	int		has_vale;

	has_vale = 0;
	if (current->vale != NULL)
		has_vale = 1;
	new_node = create_new_node(current->name, current->vale, has_vale);
	if (!new_node)
		return (NULL);
	if (*last_node)
		(*last_node)->next = new_node;
	*last_node = new_node;
	return (new_node);
}

t_envi	*copy_list(t_envi *env)
{
	t_envi	*new_env;
	t_envi	*current;
	t_envi	*last_node;
	t_envi	*new_node;

	init_copy_env(&new_env, &last_node);
	current = env;
	while (current)
	{
		new_node = process_env_node(current, &last_node);
		if (!new_node)
		{
			free_env(new_env);
			return (NULL);
		}
		if (!new_env)
			new_env = new_node;
		current = current->next;
	}
	return (new_env);
}
