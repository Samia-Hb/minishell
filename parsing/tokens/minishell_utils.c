/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 16:52:29 by shebaz            #+#    #+#             */
/*   Updated: 2024/10/23 18:59:49 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_operator(Token *node)
{
	if (node->type == TOKEN_DOUBLE_QUOTED || node->type == TOKEN_SINGLE_QUOTED
		|| node->type == TOKEN_COMMAND || node->type == TOKEN_OPTION
		|| node->type == TOKEN_BUILT_IN || node->type == TOKEN_UNKNOWN
		|| node->type == TOKEN_ARGUMENT)
		return (0);
	return (1);
}

int	is_operand(Token *node)
{
	if (node->type == TOKEN_UNKNOWN || node->type == TOKEN_DOUBLE_QUOTED
		|| node->type == TOKEN_SINGLE_QUOTED || node->type == TOKEN_COMMAND
		|| node->type == TOKEN_BUILT_IN || node->type == TOKEN_ARGUMENT
		|| node->type == TOKEN_OPTION)
		return (1);
	return (0);
}

t_stack	*push_stack(t_stack *stack, Token *node)
{
	t_stack	*head;

	head = malloc(sizeof(t_stack));
	if (!head)
		return (NULL);
	head->node = malloc(sizeof(Token));
	if (!head->node)
	{
		free(head);
		return (NULL);
	}
	head->node->token = node;
	head->next = stack;
	return (head);
}

t_stack	*pop_stack(t_stack **stack)
{
	t_stack	*node;

	if (!stack || !*stack)
		return (NULL);
	node = *stack;
	*stack = (*stack)->next;
	node->next = NULL;
	return (node);
}
