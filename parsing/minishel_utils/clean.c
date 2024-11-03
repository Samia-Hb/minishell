/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 13:39:34 by shebaz            #+#    #+#             */
/*   Updated: 2024/10/28 12:13:54 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// void	free_tokens(Token *tokens)
// {
// 	Token	*tmp;
// 	int		i;

// 	tmp = tokens;
// 	while (tokens)
// 	{
// 		tokens = tokens->next;
// 		if (tmp->value)
// 			free(tmp->value);
// 		if (tmp->expanded_value)
// 		{
// 			i = 0;
// 			while (tmp->expanded_value[i])
// 			{
// 				free(tmp->expanded_value[i]);
// 				i++;
// 			}
// 			free(tmp->expanded_value);
// 		}
// 		free(tmp);
// 	}
// }

// void	free_token(Token *token)
// {
// 	int	i;

// 	if (token)
// 	{
// 		printf("yes\n");
// 		while (token)
// 		{
// 			if (token->value)
// 				free(token->value);
// 			if (token->expanded_value)
// 			{
// 				i = 0;
// 				while (token->expanded_value[i])
// 				{
// 					free(token->expanded_value[i]);
// 					i++;
// 				}
// 				free(token->expanded_value);
// 			}
// 			token = token->next;
// 		}
// 	}
// }
