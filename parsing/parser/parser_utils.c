/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 20:24:14 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/22 01:46:59 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_token	*get_last_token(t_token *token)
{
	if (token == NULL)
		return (NULL);
	while (token->next != NULL)
		token = token->next;
	return (token);
}

int	check_quote(char *str)
{
	int		i;
	char	quote;

	i = 0;
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			quote = str[i];
			i++;
			while (str[i] && str[i] != quote)
				i++;
			if (str[i] && str[i] == quote)
				i++;
			else
				return (0);
		}
		else
			i++;
	}
	return (1);
}

int	handle_quotes(t_token *tokens)
{
	while (tokens)
	{
		if (!check_quote(tokens->value))
		{
			printf("Syntax Error: unclosed quoted\n");
			return (1);
		}
		tokens = tokens->next;
	}
	return (0);
}

int	handle_paren(t_token *token)
{
	t_token	*current;
	int		open_parentheses;

	current = token;
	while (current)
	{
		if (!ft_strcmp("()", token->value))
		{
			printf("Syntax Error.\n");
			return (1);
		}
		current = current->next;
	}
	current = token;
	open_parentheses = 0;
	if (current && current->type == TOKEN_OPEN_PARENTH)
		open_parentheses++;
	else if (current && current->type == TOKEN_CLOSE_PARENTH)
		open_parentheses--;
	if (open_parentheses < 0)
	{
		printf("Syntax Error.\n");
		return (1);
	}
	return (0);
}

int	handle_operator_expand(t_token **token)
{
	t_token	*tmp;

	(*token) = (*token)->next;
	tmp = *token;
	expand(tmp);
	return (1);
}
