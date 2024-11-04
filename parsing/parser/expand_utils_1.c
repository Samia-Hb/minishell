/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 20:13:54 by shebaz            #+#    #+#             */
/*   Updated: 2024/11/02 15:35:47 by shebaz           ###   ########.fr       */
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

char	*tidle_expansion(int *i)
{
	char	*result;

	result = getenv("HOME");
	if (!result)
		return (ft_strdup(""));
	*i += 1;
	return (ft_strdup(result));
}

int dollar_counter(char *input)
{
	int i;
	int count;

	count = 0;
	i = 0;	
	while (input[i] == '$')
	{
		count++;
		i++;
	}
	return (count);
}

char	*dollar_expand(char *input, int *i)
{
	char	*word;
	char	*result;
	int		dollar_count;
	int		j;

	result = ft_strdup("");
	dollar_count = dollar_counter(input);
	if (dollar_count == 1 && ft_strlen(input) == 1)
	{
		*i += ft_strlen(input);	
		return (input);
	}
	j = 0;
	while (j++ < dollar_count / 2)
		result = ft_strjoin(result, "$");
	word = get_word_to_expand(input + *i, i);
	if (!word)
		return (ft_strdup(""));
	if (!(dollar_count % 2))
		result = ft_strjoin(result, word);
	else
	{
		result = ft_strjoin(result, getenv(word));
		if (!result)
		{
			free(word);
			result = ft_strdup("");
		}
	}
	free(word);
	return (ft_strdup(result));
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
