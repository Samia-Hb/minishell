/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 12:15:32 by szeroual          #+#    #+#             */
/*   Updated: 2024/12/03 22:13:41 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// void    add_node(void *data)
// {
//     t_gc    *node;

//     node = malloc(1 * sizeof(t_gc));
//     if (!node)
//         return;
//     node->ptr = data;
//     node->next = NULL;
//     if (!g_var)
//     {
//         if (!g_var)
//         {
//             free(node);
//             return;
//         }
//         g_var->head = node;
//     }
//     else
//     {
//         node->next = g_var->head;
//         g_var->head = node;
//     }
// }

void	*ft_calloc(size_t count, size_t size)
{
	size_t	total_size;
	void	*result;

	if (size != 0 && count > SIZE_MAX / size)
		return (NULL);
	total_size = count * size;
	result = malloc(total_size);
	if (result != NULL)
		ft_memset(result, 0, total_size);
	add_node(result);
	return (result);
}
