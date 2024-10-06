/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 13:16:22 by szeroual          #+#    #+#             */
/*   Updated: 2024/10/06 11:29:18 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <stdio.h>
//#include <string.h>
// #include <stdlib.h>
#include "libft.h"

static void	ft_strcpy(char *destination, const char *source)
{
	while (*source != '\0')
	{
		*destination = *source;
		source++;
		destination++;
	}
	*destination = '\0';
}

static void	ft_strcat(char *dest, const char *src)
{
	while (*dest != '\0')
	{
		dest++;
	}
	while (*src != '\0')
	{
		*dest = *src;
		dest++;
		src++;
	}
	*dest = '\0';
}

char	*ft_strjoin(const char *s1, const char *s2)
{
	char	*result;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	else if (!s2)
		return (ft_strdup(s1));
	result = (char *)malloc((ft_strlen(s2) + ft_strlen(s1) + 1) * sizeof(char));
	if (result != NULL)
	{
		ft_strcpy(result, s1);
		ft_strcat(result, s2);
	}
	return (result);
}

// #include <stdio.h>

// int main(int argc, char **argv)
// {
// 	(void)argc;
// 	char *result = ft_strjoin(argv[1], argv[2]);
// 	printf("result = %s\n", result);
// 	free(result);
// }
