/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 21:02:27 by szeroual          #+#    #+#             */
/*   Updated: 2024/12/03 11:58:46 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	size_t	s_len;
	char	*result;
	size_t	i;

	i = 0;
	if (s == NULL || f == NULL)
		return (NULL);
	s_len = ft_strlen(s);
	result = (char *)ft_calloc(s_len + 1, sizeof(char));
	if (result != NULL)
	{
		while (i < s_len)
		{
			result[i] = f(i, s[i]);
			i++;
		}
		result[s_len] = '\0';
	}
	return (result);
}
