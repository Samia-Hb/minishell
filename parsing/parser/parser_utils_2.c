/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 14:00:16 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/20 03:56:12 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*tidle_expansion(int *i)
{
	char	*result;

	result = getenv("HOME");
	if (!result)
		return (ft_strdup(""));
	*i += 1;
	return (ft_strdup(result));
}

char	*ft_getenv(char *word)
{
	t_envi	*tmp;

	tmp = g_var->envp;
	if (tmp)
	{
		while (tmp)
		{
			if (tmp->name && !ft_strcmp(tmp->name, word))
				return (ft_strdup(tmp->vale));
			tmp = tmp->next;
		}
	}
	return (NULL);
}

int	is_number(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	check_symbol_nbr(char *str)
{
	int	i;
	int	counter;

	i = 0;
	counter = 0;
	while (str[i])
	{
		if (str[i] == '<')
			counter++;
		i++;
	}
	if (counter > 2)
	{
		printf("minishell: syntax error near unexpected token.\n");
		return (1);
	}
	return (0);
}

int	file_expansion_null(char **filename)
{
	char	**value;
	char	*result;

	result = expand_non_operator(*filename);
	value = result_traitement(result);
	if (!ft_strlen(result) || !value[0] || value[1])
	{
		g_var->exit_status = 1;
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(*filename, 2);
		ft_putstr_fd(": ambiguous redirect\n", 2);
		return (1);
	}
	else
		*filename = value[0];
	return (0);
}
