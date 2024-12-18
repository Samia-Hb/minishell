/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.utils0.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 10:18:48 by szeroual          #+#    #+#             */
/*   Updated: 2024/12/18 11:03:22 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

void	free_env(t_envi *env)
{
	t_envi	*temp;

	while (env != NULL)
	{
		temp = env;
		env = env->next;
		free(temp->name);
		free(temp->vale);
		free(temp);
	}
}

void	free_env_array(char **env_array)
{
	int	i;

	if (!env_array)
		return ;
	i = 0;
	while (env_array[i])
	{
		free(env_array[i]);
		i++;
	}
	free(env_array);
}

t_envi	*add_new_var(const char *var, const char *vale, int had_equals)
{
	t_envi	*new_node;

	new_node = (t_envi *)malloc(sizeof(t_envi));
	if (!new_node)
	{
		perror("malloc");
		return (NULL);
	}
	new_node->name = ft_strdup_1(var);
	if (had_equals == 1)
		new_node->vale = set_node_vale(vale);
	else
		new_node->vale = NULL;
	if (!new_node->name || (had_equals && !new_node->vale))
	{
		free_node_contents(new_node);
		return (NULL);
	}
	new_node->next = NULL;
	return (new_node);
}

void	update_or_add_env_part1(t_envi **env, const char *var, t_envi **prev,
		t_envi **current)
{
	*current = *env;
	*prev = NULL;
	while (*current)
	{
		if (ft_strcmp((*current)->name, var) == 0)
			return ;
		*prev = *current;
		*current = (*current)->next;
	}
}

void	update_or_add_env(t_envi **env, const char *var, const char *vale,
		int had_equals)
{
	t_envi	*current;
	t_envi	*prev;
	t_envi	*new_node;

	update_or_add_env_part1(env, var, &prev, &current);
	if (current)
	{
		update_existing_node(current, vale, had_equals);
		return ;
	}
	new_node = add_new_var(var, vale, had_equals);
	if (!new_node)
		return ;
	if (prev)
		prev->next = new_node;
	else
		*env = new_node;
}
