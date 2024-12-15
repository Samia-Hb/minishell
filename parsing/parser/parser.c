/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 13:46:11 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/15 00:16:42 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	dollar_counter(char *input)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (input[i] == '$')
	{
		count++;
		i++;
	}
	return (count);
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
	}
	return (size);
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

int	random_case(t_token *tokens)
{
	while (tokens)
	{
		if ((is_red(tokens) && tokens->type != TOKEN_REDIR_HERE_DOC)
			&& (ft_strchr_sec(tokens->next->value, '(')
				|| ft_strchr_sec(tokens->next->value, ')')))
		{
			printf("minishell: syntax error near unexpected token.\n");
			return (1);
		}
		tokens = tokens->next;
	}
	return (0);
}

int	check_syntax_errors(t_token *tokens)
{
	int	tmp;

	tmp = 0;
	if (handle_quotes(tokens))
		tmp = 1;
	if (handle_paren(tokens))
		tmp = 1;
	if (handle_operators_bg_en(tokens))
		tmp = 1;
	if (handle_consecutive_operator(tokens))
		tmp = 1;
	if (random_case(tokens))
		tmp = 1;
	if (tmp == 1)
		g_var->exit_status = 2;
	return (tmp);
}
