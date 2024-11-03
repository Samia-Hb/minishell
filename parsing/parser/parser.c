/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 13:46:11 by shebaz            #+#    #+#             */
/*   Updated: 2024/11/02 16:11:30 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	update_tokens(Token **tokens)
{
	Token	*current;
	char	quote;

	current = *tokens;
	while (current)
	{
		if (current->type == TOKEN_DOUBLE_QUOTED
			|| current->type == TOKEN_SINGLE_QUOTED)
		{
			quote = quote_type(current->value);
			if (check_token(current->value, quote))
			{
				current->value = ft_strtrim(current->value,
											char_to_string(quote, 0));
			}
		}
		current = current->next;
	}
}

int	ft_strchr_sec(char *string, char c)
{
	int	i;

	i = 0;
	while (string[i])
	{
		if (string[i] == c)
			return (1);
		i++;
	}
	return (0);
}

int	random_case(Token *tokens)
{
	while (tokens)
	{
		if ((is_red(tokens) && tokens->type != TOKEN_REDIR_HERE_DOC)
			&& (ft_strchr_sec(tokens->next->value, '(')
				|| ft_strchr_sec(tokens->next->value, ')')))
		{
			printf("Syntax Error.\n");
			return (1);
		}
		tokens = tokens->next;
	}
	return (0);
}

int	check_syntax_errors(Token *tokens)
{
	if (handle_quotes(tokens))
		return (1);
	if (handle_paren(tokens))
		return (2);
	if (handle_operators_bg_en(tokens))
		return (5);
	if (handle_consecutive_operator(tokens))
		return (4);
	if (random_case(tokens))
		return (2);
	return (0);
}
