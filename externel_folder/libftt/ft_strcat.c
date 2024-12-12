/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 22:50:52 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/12 23:40:38 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strcat(char *dest, const char *src)
{
	int		size;
	char	*result;
	int		i;
	int		j;

	i = 0;
	size = ft_strlen(dest) + ft_strlen(src) + 1;
	result = malloc(size * sizeof(char));
	if (!result)
		return (NULL);
	while (i < size && dest[i])
	{
		result[i] = dest[i];
		i++;
	}
	j = 0;
	while (i < size && src[j])
	{
		result[i] = src[j];
		i++;
		j++;
	}
	result[i] = '\0';
	return (result);
}
