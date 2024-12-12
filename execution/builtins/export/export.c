/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanaa <sanaa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 21:41:35 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/12 21:05:24 by sanaa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

void	swap_nodes(t_envi *a, t_envi *b)
{
	char	*temp_name;
	char	*temp_vale;

	temp_name = a->name;
	temp_vale = a->vale;
	a->name = b->name;
	a->vale = b->vale;
	b->name = temp_name;
	b->vale = temp_vale;
}
int	is_valid_identifier(const char *str)
{
	if (!str || !*str || !(ft_isalpha(*str) || *str == '_'))
		return (0);
	while (*++str)
		if (!(ft_isalnum(*str) || *str == '_'))
			return (0);
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

	env_copy = copy_env(*env);
	if (!env_copy)
		return ;
	print_export(env_copy);
	// free_env(env_copy);
}

void	ft_export(t_envi **env, char **cmd)
{
	int	i;

	g_var->exit_status = 0;
	if (cmd[1] == NULL)
	{
		handle_no_cmd(env);
		return ;
	}
	i = 1;
	while (cmd[i])
	{
		printf("cmd[1] = %s\n", cmd[i]);
		process_cmd(env, cmd, i);
		i++;
	}
	sync_env_array(*env);
}