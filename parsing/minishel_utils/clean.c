/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 13:39:34 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/03 13:23:48 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	*ft_malloc(size_t size, int ele_nbr)
{
	t_gc	*node;
	void	*ptr;

	ptr = malloc(ele_nbr * size);
	node = malloc(sizeof(t_gc));
	node->ptr = ptr;
	node->next = NULL;
	if (!g_var)
	{
		g_var = malloc(sizeof(struct s_global));
		g_var->head = node;
	}
	else
	{
		node->next = g_var->head;
		g_var->head = node;
	}
	return (ptr);
}


void clean_gc(void)
{
    t_gc *current;
    t_gc *next;

    if (!g_var)
        return;
    current = g_var->head;
    while (current)
    {
        next = current->next;
        if (current->ptr)
            free(current->ptr);
        free(current);         
        current = next;
    }
	if (g_var->envp)
	{
		t_envi *n = g_var->envp;
		t_envi *n1 = g_var->envp->next; 
		while (n)
		{
			if(g_var->envp->next)
				n1 = g_var->envp->next; 
			if (n->name)
				free(n->name);
			if (n->vale)
				free(n->vale);
			free(n);      
			n = n1;
		}
	}
	if (g_var->en)
		ft_free_array(g_var->en);
    free(g_var);
}
