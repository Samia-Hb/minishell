/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 22:25:31 by shebaz            #+#    #+#             */
/*   Updated: 2024/11/02 16:38:13 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

char	**handle_that_shit(char *input)
{
	char	**result;
	char	*n_strimmed;
	int		size;
	int		i;
	int		j;

	i = 0;
	j = 0;
	size = get_size_arr(input);
	result = malloc((size + 1) * sizeof(char *));
	if (!result)
		printf("Allocation Failed\n");
	while (input[i])
	{
		n_strimmed = get_string(input, &i);
		result[j] = ft_strtrim(n_strimmed, " ");
		free(n_strimmed);
		j++;
	}
	result[size] = NULL;
	return (result);
}

char	**unquoted_result(char **input)
{
	char	**output;
	char	*str;
	int		i;
	int		j;
	char	c;

	i = 0;
	j = 0;
	output = malloc((get_size(input) + 1) * sizeof(char *));
	while (input[i])
	{
		j = 0;
		output[i] = ft_strdup("");
		while (input[i][j])
		{
			if (input[i][j] == '"' || input[i][j] == '\'')
			{
				c = input[i][j];
				j++;
				while (input[i][j] != c)
				{
					str = char_to_string(input[i][j], 0);
					output[i] = ft_strjoin(output[i], str);
					j++;
				}
				j++;
			}
			else
			{
				str = char_to_string(input[i][j], 0);
				output[i] = ft_strjoin(output[i], str);
				j++;
			}
		}
		i++;
	}
	output[i] = NULL;
	return (output);
}

int	handle_operator_expand(Token **token)
{
	Token	*tmp;

	(*token) = (*token)->next;
	tmp = *token;
	expand(tmp);
	if (tmp->expanded_value)
	{
		if (get_size(tmp->expanded_value) > 1)
		{
			return (0);
		}
	}
	return (1);
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
	free(tmp);
	return (result);
}

int	expand(Token *tokens)
{
	char	*result;

	while (tokens)
	{
		result = ft_strdup("");
		tokens->expanded_value = NULL;
		if (tokens->type == TOKEN_REDIR_HERE_DOC)
			tokens = tokens->next;
		else
		{
			if (is_red(tokens))
			{
				if (!handle_operator_expand(&tokens))
					return (printf("Error\n"), 0);
			}
			if (!is_operator(tokens))
			{
				// printf("token_value = %s\n", tokens->value);
				result = expand_non_operator(tokens->value);
				// printf("expand_result ========> %s\n", result);
				// exit(1);
				tokens->expanded_value = result_traitement(result);
			}
		}
		free(result);
		tokens = tokens->next;
	}
	return (1);
}
