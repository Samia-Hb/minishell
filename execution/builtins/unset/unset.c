/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szeroual <szeroual@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 16:02:59 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/05 23:43:33 by szeroual         ###   ########.fr       */
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

int	ft_unset(char **ptr, t_envi **envi)
{
	int	j;
	int	i;

	j = 1;
	i = 1;
	if (!ptr || !ptr[1] || !envi)
		return (0);
	while (ptr[i])
	{
		if (ptr[1])
			ft_remove(ptr, envi, j);
		else
			printf("unset: '%s': not a valid identifier\n", ptr[i]);
		i++;
	}
	return (0);
}
