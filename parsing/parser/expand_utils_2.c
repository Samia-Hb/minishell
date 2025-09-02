/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 21:17:57 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/21 03:19:22 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	get_size(char **arr)
{
	int	size;

	size = 0;
	while (arr[size])
		size++;
	return (size);
}

int	is_special(char c)
{
	if (((c >= 32 && c <= 47) || (c >= 58 && c <= 64) || (c >= 91 && c <= 96)
			|| (c >= 123 && c <= 126)) && (c != '*') && (c != '_'))
		return (1);
	return (0);
}

char	*double_quote_expansion(char *input, int *i)
{
	char	*expanded_value;
	char	*result;

	expanded_value = ft_strdup("");
	if (!expanded_value)
		return (NULL);
	if (input[*i] == '"')
		add_quote(input + (*i), &expanded_value, i);
	while (input[*i] && input[*i] != '"')
	{
		if (input[*i] == '$')
			result = dollar_expand(input, i);
		else
		{
			result = char_to_string(input[*i], 0);
			(*i)++;
		}
		expanded_value = ft_strjoin(expanded_value, result);
	}
	if (input[*i] == '"')
		add_quote(input + (*i), &expanded_value, i);
	return (expanded_value);
}

char	*single_quote_expansion(char *input, int *i)
{
	char	*expanded_value;
	char	*result;

	expanded_value = ft_strdup("");
	if (!expanded_value)
		return (NULL);
	if (input[*i] == '\'')
		add_quote(input + (*i), &expanded_value, i);
	while (input[*i] && input[*i] != '\'')
	{
		result = char_to_string(input[*i], 0);
		expanded_value = ft_strjoin(expanded_value, result);
		(*i)++;
	}
	if (input[*i] == '\'')
		add_quote(input + (*i), &expanded_value, i);
	return (expanded_value);
}

char	**result_traitement(char *input)
{
	char	**result;

	if (!strchr(input, '"') && !strchr(input, '\''))
		result = ft_split(input, ' ');
	else
		result = handle_that_shit(input);
	result = unquoted_result(result);
	return (result);
}
