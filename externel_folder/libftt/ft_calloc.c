/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 12:15:32 by szeroual          #+#    #+#             */
/*   Updated: 2024/12/03 12:50:19 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// void	adde_node(void *data)
// {
// 	t_gc	*node;

// 	node = malloc(1 * sizeof(t_gc));
// 	node->ptr = data;
// 	if (!g_var)
// 	{
// 		g_var = ft_calloc(1, sizeof(struct s_global));
// 		g_var->head = node;
// 	}
// 	else
// 	{
// 		node->next = g_var->head;
// 		g_var->head = node;
// 	}
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
