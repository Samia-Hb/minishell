/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 22:25:31 by shebaz            #+#    #+#             */
/*   Updated: 2024/10/23 22:24:06 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*get_word_to_expand(char *str)
{
	char	*word;
	int		length;
	int		i;

	i = 0;
	length = 0;
	while (str[i] && ((str[i] <= 'z' && str[i] >= 'a') || (str[i] <= 'Z'
				&& str[i] >= 'A') || (str[i] >= '0' && str[i] <= '9')))
	{
		i++;
		length++;
	}
	word = malloc(length + 1);
	if (!word)
		return (NULL);
	i = 0;
	while (i < length)
	{
		word[i] = str[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

char	*tidle_expansion(int *i)
{
	char	*result;

	result = getenv("HOME");
	if (!result)
		return (ft_strdup(""));
	*i += 1;
	return (ft_strdup(result));
}

char	*dollar_expand(char *input, int *i)
{
	char	*word;
	char	*result;

	word = get_word_to_expand(input + *i + 1);
	if (!word)
		return (ft_strdup(""));
	result = getenv(word);
	if (!result)
	{
		free(word);
		result = ft_strdup("");
	}
	*i += ft_strlen(word) + 1;
	free(word);
	return (ft_strdup(result));
}

void add_quote(char *input, char **expanded_value, int *j)
{
	int		i;
	char 	*result;

	i = 0;
	result = char_to_string(input[i], 0);
	*expanded_value = ft_strjoin(*expanded_value, result);
	if (result)
		free(result);
	(*j)++;
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
	if (result)
		free(result);
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
	if (result)
		free(result);
	return (expanded_value);
}

char	*get_word(char **string, int *counter)
{
	char	*result;
	int		i;

	i = 0;
	if ((*counter) > 1)
	{
		while ((*string)[i] && ((*string)[i] == ' ' || (*string)[i] == '\t'))
			i++;
		while ((*string)[i] && ((*string)[i] != ' ' && (*string)[i] != '\t'))
			i++;
		result = ft_strndup(*string, i);
		(*string) += i;
	}
	else
		result = ft_strndup((*string), ft_strlen(*string));
	(*counter)--;
	return (result);
}

char	*get_output(char *input)
{
	char	*str;
	int		i;
	int		k;

	k = 0;
	i = 0;
	str = NULL;
	while (input[i] == ' ' || input[i] == '\t')
	{
		i++;
		k++;
	}
	i = k;
	while (input[i] && !ft_is_separator(input[i]))
	{
		if (input[i] == '"' || input[i] == '\'')
		{
			str = handle_quote(input + i);
			i += ft_strlen(str);
		}
		i++;
	}
	if (str)
		free(str);
	return (ft_strndup(input, i));
}

char	*get_string(char *input, int *i)
{
	char	*word;

	if (input[*i] == '"' || input[*i] == '\'')
	{
		word = handle_quote(input + (*i));
		*i += ft_strlen(word);
	}
	else
	{
		word = get_output(input + (*i));
		*i += ft_strlen(word);
	}
	return (word);
}

int	get_size_arr(char *input)
{
	int		i;
	char	*word;
	int		size;
	char	*n_strimmed;

	i = 0;
	size = 0;
	while (input[i])
	{
		n_strimmed = get_string(input, &i);
		word = ft_strtrim(n_strimmed, " ");
		if (ft_strlen(word) > 0)
			size++;
		free(word);
		free(n_strimmed);
	}
	return (size);
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

// char *check_input(char *input)
// {
// 	int		i;
// 	char	*output;

// 	i = 0;
// 	output = strdup("");
// 	while (input[i])
// 	{
// 		if (input[i] == '$')
// 			output = ft_strjoin(output, dollar_expand(input + i, &i));
// 		else
// 		{
// 			output = ft_strjoin(output, char_to_string(input[i], 0));
// 			i++;
// 		}
// 	}
// 	return (output);
// }
int	get_size(char **arr)
{
	int	size;

	size = 0;
	while (arr[size])
		size++;
	return (size);
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
		free(str);
		i++;
	}
	output[i] = NULL;
	return (output);
}

char	**result_traitement(char *input)
{
	char	**result;

	if (!ft_strchr(input, '"') && !ft_strchr(input, '\''))
		result = ft_split(input, ' ');
	else
		result = handle_that_shit(input);
	result = unquoted_result(result);
	return (result);
}
void	expand(Token *tokens)
{
	char	*result;
	int		i;
	char	*tmp;

	while (tokens)
	{
		i = 0;
		result = ft_strdup("");
		if (!result)
			return ;
		tokens->expanded_value = NULL;
		if (!is_operator(tokens))
		{
			while (tokens->value[i])
			{
				if (tokens->value[i] == '"')
					tmp = double_quote_expansion(tokens->value, &i);
				else if (tokens->value[i] == '\'')
					tmp = single_quote_expansion(tokens->value, &i);
				else if (tokens->value[i] == '~')
					tmp = tidle_expansion(&i);
				else if (tokens->value[i] == '$')
					tmp = dollar_expand(tokens->value, &i);
				else
				{
					tmp = char_to_string(tokens->value[i], 0);
					i++;
				}
				result = ft_strjoin(result, tmp);
				free(tmp);
			}
			tokens->expanded_value = result_traitement(result);
		}
		free(result);
		tokens = tokens->next;
	}
}
