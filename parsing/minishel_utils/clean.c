/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 13:39:34 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/03 13:06:02 by shebaz           ###   ########.fr       */
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

void	clean_gc(void)
{
	t_gc	*temp;

	t_gc *var =  g_var->head;
	while (var)
	{
		if (var->ptr)
			free(var->ptr);
		temp = var;
		if (var)
			free(var);
		var = var->next;
	}
	// if(var)
	// 	free(var);
	// if(temp)
	// 	free(temp);
}
// if (g_var->head)
// 	free(g_var->head);
// if (g_var)
// 	free(g_var);

void clean_gc(void)
{
    t_gc *current;
    t_gc *next;

    if (!g_var)
        return; // Nothing to clean if g_var is NULL

    current = g_var->head;
    while (current)
    {
        next = current->next; // Save the next node
        if (current->ptr)
            free(current->ptr); // Free the allocated memory
        free(current);          // Free the current node
        current = next;         // Move to the next node
    }

    g_var->head = NULL; // Reset the head pointer
    free(g_var);        // Free the global structure itself
    g_var = NULL;       // Avoid dangling pointer
}
