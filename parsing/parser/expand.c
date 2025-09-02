/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 22:25:31 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/21 03:26:28 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*get_unquoted_word(char **input, char **result, int *i, int *j)
{
	char	*str;
	char	c;

	*result = ft_strdup("");
	while (input[*i][*j])
	{
		if (input[*i][*j] == '"' || input[*i][*j] == '\'')
		{
			c = input[*i][*j];
			(*j)++;
			while (input[*i][*j] != c)
			{
				str = char_to_string(input[*i][*j], 0);
				*result = ft_strjoin(*result, str);
				(*j)++;
			}
		}
		else
		{
			str = char_to_string(input[*i][*j], 0);
			*result = ft_strjoin(*result, str);
		}
		(*j)++;
	}
	return (*result);
}

char	**unquoted_result(char **input)
{
	char	**output;
	int		i;
	int		j;

	i = 0;
	j = 0;
	output = ft_malloc(get_size(input) + 1, sizeof(char *));
	while (input[i])
	{
		j = 0;
		output[i] = ft_strdup("");
		output[i] = get_unquoted_word(input, &output[i], &i, &j);
		i++;
	}
	output[i] = NULL;
	return (output);
}

char	*expand_non_operator(char *token)
{
	char	*result;
	char	*tmp;
	int		i;

	result = ft_strdup("");
	i = 0;
	while (token[i])
	{
		if (token[i] == '"')
			tmp = double_quote_expansion(token, &i);
		else if (token[i] == '\'')
			tmp = single_quote_expansion(token, &i);
		else if (token[i] == '~')
			tmp = tidle_expansion(&i);
		else if (token[i] == '$')
			tmp = dollar_expand(token, &i);
		else
		{
			tmp = char_to_string(token[i], 0);
			i++;
		}
		result = ft_strjoin(result, tmp);
	}
	return (result);
}

char	**is_operator_case(t_token *tokens)
{
	char	*result;

	result = ft_strdup("");
	if (!ft_strchr(tokens->value, '"') && !ft_strchr(tokens->value, '\''))
	{
		result = expand_non_operator(tokens->value);
		tokens->expanded_value = ft_split(result, ' ');
	}
	else
	{
		result = expand_non_operator(tokens->value);
		tokens->expanded_value = result_traitement(result);
	}
	return (tokens->expanded_value);
}

int	expand(t_token *tokens)
{
	while (tokens)
	{
		tokens->expanded_value = NULL;
		if (tokens->type == TOKEN_REDIR_HERE_DOC)
			tokens = tokens->next;
		else
		{
			if (is_red(tokens))
			{
				if (!handle_operator_expand(&tokens))
					return (0);
			}
			if (!is_operator(tokens))
			{
				tokens->expanded_value = is_operator_case(tokens);
			}
		}
		tokens = tokens->next;
	}
	return (1);
}
