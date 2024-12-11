/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 14:00:16 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/11 22:13:55 by shebaz           ###   ########.fr       */
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
	while (tmp)
	{
		if (!strcmp(tmp->name, word))
			return (ft_strdup(tmp->vale));
		tmp = tmp->next;
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

int	file_expansion_null(t_token *tokens)
{
	char	**value;
	char	*result;

	while (tokens)
	{
		if (tokens->type == TOKEN_REDIR_HERE_DOC)
		{
			if (check_symbol_nbr(tokens->value))
				return (1);
		}
		if (is_red(tokens) && tokens->type != TOKEN_REDIR_HERE_DOC)
		{
			result = expand_non_operator(tokens->next->value);
			value = result_traitement(result);
			if (!ft_strlen(result) || !value[0])
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(tokens->next->value, 2);
				ft_putstr_fd(": ambiguous redirect\n", 2);
				return (1);
			}
		}
		tokens = tokens->next;
	}
	return (0);
}
