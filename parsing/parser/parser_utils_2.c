/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 14:00:16 by shebaz            #+#    #+#             */
/*   Updated: 2024/11/09 14:00:30 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	file_expansion_null(t_token *tokens)
{
	char	**value;
	char	*result;

	while (tokens)
	{
		if (is_red(tokens) && tokens->type != TOKEN_REDIR_HERE_DOC)
		{
			result = expand_non_operator(tokens->next->value);
			value = result_traitement(result);
			if (!ft_strlen(result) || !value[0])
			{
				printf("Error\n");
				return (1);
			}
		}
		tokens = tokens->next;
	}
	return (0);
}
