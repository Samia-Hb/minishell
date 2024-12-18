/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 20:54:45 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/05 13:07:41 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s1, int n)
{
	char	*s2;
	int		i;

	i = 0;
	s2 = (char *)ft_malloc(n + 1, sizeof(char));
	if (s2 == NULL)
	{
		return (NULL);
	}
	while (s1[i] != '\0' && i < n)
	{
		s2[i] = s1[i];
		i++;
	}
	s2[i] = '\0';
	return (s2);
}
