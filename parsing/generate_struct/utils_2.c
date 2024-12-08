/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 17:12:31 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/05 20:39:05 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ctrl_c(int nb)
{
	if (nb == SIGINT)
	{
		printf("\n");
		clean_gc();
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

char	*generate_name(int *i)
{
	char	*filename;
	char	*str;

	filename = NULL;
	str = ft_itoa(*i);
	if (access("tmp", F_OK))
		filename = ft_strjoin("tmp", ft_itoa(*i));
	else if (access("tmp", F_OK) == 0)
	{
		while (access("tmp", F_OK))
		{
			str = ft_strjoin(str, str);
		}
		filename = ft_strjoin("tmp", ft_itoa(*i));
	}
	(*i)++;
	return (filename);
}

void	exit_status(int status, char *filename)
{
	if (WEXITSTATUS(status) == 7)
	{
		g_var->exit_status = 130;
		unlink(filename);
		g_var->flag = 7;
	}
}
