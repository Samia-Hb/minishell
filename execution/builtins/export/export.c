/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 10:18:44 by szeroual          #+#    #+#             */
/*   Updated: 2024/12/23 23:10:29 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	populate_env_array(char **env_array, t_envi *env, int count)
{
	int		i;
	t_envi	*current;

	i = 0;
	current = env;
	while (current && i < count)
	{
		if (current->vale != NULL)
		{
			env_array[i] = create_env_string(current);
			if (!env_array[i])
				return (0);
			i++;
		}
		current = current->next;
	}
	env_array[i] = NULL;
	return (1);
}

t_envi	*bubble_sort_env(t_envi *env)
{
	int		swapped;
	t_envi	*ptr1;
	t_envi	*lptr;

	if (!env)
		return (NULL);
	lptr = NULL;
	while (1)
	{
		swapped = 0;
		ptr1 = env;
		while (ptr1->next != lptr)
		{
			if (ft_strcmp(ptr1->name, ptr1->next->name) > 0)
			{
				swap_nodes(ptr1, ptr1->next);
				swapped = 1;
			}
			ptr1 = ptr1->next;
		}
		lptr = ptr1;
		if (!swapped)
			break ;
	}
	return (env);
}

void	print_export(t_envi *env)
{
	t_envi	*current;

	current = bubble_sort_env(env);
	while (current)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(current->name, 1);
		if (current->vale != NULL)
		{
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(current->vale, 1);
			ft_putstr_fd("\"", 1);
		}
		ft_putstr_fd("\n", 1);
		current = current->next;
	}
}

void	handle_no_cmd(t_envi **env)
{
	t_envi	*env_copy;

	env_copy = copy_list(*env);
	if (!env_copy)
		return ;
	print_export(env_copy);
	ft_free_envp(env_copy);
}

void	ft_export(char **cmd)
{
	int	i;

	g_var->exit_status = 0;
	if (cmd[1] == NULL)
	{
		handle_no_cmd(&g_var->envp);
		return ;
	}
	i = 1;
	while (cmd[i])
	{
		process_cmd(&g_var->envp, cmd, i);
		i++;
	}
	sync_env_array(g_var->envp);
}
