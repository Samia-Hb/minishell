/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 20:13:54 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/21 03:18:28 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	rare_case(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if ((input[i] >= 'a' && input[i] <= 'z') || (input[i] >= 'A'
				&& input[i] <= 'Z') || input[i] != '"' || input[i] != '\'')
			return (0);
		i++;
	}
	return (1);
}

char	*expand_cases(char *input, int dollar_count, int *i, int *flag)
{
	char	*result;
	int		j;

	result = ft_strdup("");
	if (rare_case(input + *i))
	{
		result = ft_strdup(input + *i);
		(*i) += ft_strlen(input + *i);
		return (result);
	}
	j = 0;
	result = ft_strdup("");
	if (dollar_count > 1)
	{
		while (j++ < dollar_count / 2)
			result = ft_strjoin(result, "$");
	}
	if (dollar_count == 1 && is_special(input[*i + 1]) && input[*i + 1] != '"'
		&& input[*i + 1] != '\'' && input[*i + 1] != '?')
	{
		result = ft_strjoin(result, "$");
		(j)++;
	}
	exit_status_case(input, &result, i, flag);
	return (result);
}

char	*handle_thing(char *word, char *result)
{
	char	*output;

	if (!ft_strncmp(word, "?", 1))
		output = ft_strjoin(result, word);
	else
		output = ft_strjoin(result, ft_getenv(word));
	if (!output)
		output = ft_strdup("");
	return (output);
}

char	*dollar_expand(char *input, int *i)
{
	char	*word;
	char	*result;
	int		flag;
	int		dollar_count;

	flag = 0;
	dollar_count = dollar_counter(input + *i);
	if (one_dollar_test_case(dollar_count, input + *i, i))
		return ("$");
	result = expand_cases(input, dollar_count, i, &flag);
	if (flag)
	{
		g_var->exit_status = 0;
		return (result);
	}
	word = get_word_to_expand(input, i, &result);
	if (!(dollar_count % 2))
		result = ft_strjoin(result, word);
	else
		result = handle_thing(word, result);
	g_var->exit_status = 0;
	return (ft_strdup(result));
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
	result = ft_malloc((size + 1), sizeof(char *));
	if (!result)
		printf("Allocation Failed\n");
	while (input[i])
	{
		n_strimmed = get_string(input, &i);
		result[j] = ft_strtrim(n_strimmed, " ");
		j++;
	}
	result[size] = NULL;
	return (result);
}
