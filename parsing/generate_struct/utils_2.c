/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 17:12:31 by shebaz            #+#    #+#             */
/*   Updated: 2024/11/30 13:37:23 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ctrl_c(int nb)
{
	if (nb == SIGINT)
	{
		printf("\n");
		exit(7);
	}
}

char	*process_delimiter(char *tmp)
{
	int		i;
	int		j;
	char	*result;
	char	*hp;

	i = 0;
	result = ft_strdup("");
	while (tmp[i])
	{
		if (tmp[i] == '$' && tmp[i + 1]
			&& (tmp[i + 1] == '"' || tmp[i + 1] == '\''))
			i++;
		j = i;
		if (tmp[i] == '"' || tmp[i] == '\'')
			hp = get_inside_quote(tmp, &i, &j);
		else
		{
			while (tmp[i] && tmp[i] != '"' && tmp[i] != '\'')
				i++;
			hp = ft_strndup(tmp + j, i - j);
		}
		result = ft_strjoin(result, hp);
	}
	return (result);
}
