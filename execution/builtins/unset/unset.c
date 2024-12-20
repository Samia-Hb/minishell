/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 10:19:24 by szeroual          #+#    #+#             */
/*   Updated: 2024/12/20 04:48:27 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

void	clear_memory(t_envi *curr)
{
	free(curr->name);
	free(curr->vale);
	free(curr);
}

void	ft_remove(char **ptr, t_envi **envi, int i)
{
	t_envi	*curr;
	t_envi	*prev;
	char	**av;

	av = ptr;
	while (av[i])
	{
		curr = *envi;
		prev = NULL;
		while (curr)
		{
			if (!ft_strcmp(curr->name, av[i]))
			{
				if (prev)
					prev->next = curr->next;
				else
					*envi = curr->next;
				clear_memory(curr);
				break ;
			}
			prev = curr;
			curr = curr->next;
		}
		i++;
	}
}

int	ft_unset(char **ptr)
{
	int	j;
	int	i;

	j = 1;
	i = 1;
	if (!ptr || !ptr[1] || !g_var->envp)
		return (0);
	while (ptr[i])
	{
		if (ptr[1])
			ft_remove(ptr, &g_var->envp, j);
		else
			printf("unset: '%s': not a valid identifier\n", ptr[i]);
		i++;
	}
	return (0);
}
