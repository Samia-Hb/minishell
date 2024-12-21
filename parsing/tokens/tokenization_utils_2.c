/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_utils_2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 13:37:34 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/20 23:25:00 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_token	*create_token(t_token_type type, const char *value)
{
	t_token	*token;

	token = ft_malloc(1, sizeof(t_token));
	if (!token)
	{
		ft_putstr_fd("Memory allocation failed\n", 2);
		exit(EXIT_FAILURE);
	}
	token->type = type;
	token->value = ft_strdup(value);
	token->next = NULL;
	return (token);
}

void	add_token(t_token **tokens, t_token_type type, char *value, int *k)
{
	t_token	*new_node;
	t_token	*ptr;

	ptr = *tokens;
	new_node = create_token(type, value);
	if (!(*tokens))
		*tokens = new_node;
	else
	{
		while (ptr->next)
			ptr = ptr->next;
		new_node->previous = ptr;
		ptr->next = new_node;
	}
	(*k) = (*k) + ft_strlen(value);
}

int	ft_is_separator(char c)
{
	if (c == '>' || c == '<' || c == '\n' || c == '\t' || c == '|' || c == ' '
		|| c == '\t')
		return (1);
	return (0);
}

char	*handle_parentheses(char *str, char c)
{
	int		i;
	int		j;
	char	*word;

	i = 1;
	j = 0;
	while (str[i] && !isspace(str[i]))
	{
		if (str[i] == '\\' && str[i + 1] == c)
			i += 2;
		else
			i++;
		j++;
	}
	word = ft_malloc(j + 1, sizeof(char));
	if (!word)
	{
		ft_putstr_fd("Memory allocation failed\n", 2);
		exit(EXIT_FAILURE);
	}
	word[0] = c;
	ft_strncpy(word + 1, str + 1, j);
	word[j] = '\0';
	return (word);
}

char	*handle_quote(char *str)
{
	int		i;
	int		j;
	char	quote;
	char	*word;

	i = 0;
	j = 0;
	word = ft_malloc(ft_strlen(str) + 1, sizeof(char));
	while (str[i] && !ft_is_separator(str[i]))
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			quote = str[i];
			word[j++] = str[i++];
			while (str[i] && str[i] != quote)
				word[j++] = str[i++];
			if (str[i] && str[i] == quote)
				word[j++] = str[i++];
		}
		else
			word[j++] = str[i++];
	}
	word[j] = '\0';
	return (word);
}
