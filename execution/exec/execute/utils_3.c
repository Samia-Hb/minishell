/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 13:42:07 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/03 11:14:50 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

void	cleanup_execute_arguments(t_cmd *token)
{
	(void)token;
	if (g_var->pre_pipe_infd > 2)
		close(g_var->pre_pipe_infd);
}

void	lista_add_front(t_alst **lst, t_alst *new)
{
	if (!new)
		return ;
	if (*lst)
		new->next = *lst;
	*lst = new;
}

t_alst	*lista_new(void *content)
{
	t_alst	*list;

	list = malloc(sizeof(t_alst));
	if (!list)
		return (NULL);
	list->content = content;
	list->next = NULL;
	return (list);
}

void	validate_cmd(t_cmd *token)
{
	if (ft_strchr(token->arguments[0], '/'))
		check_cmd_path(token);
	else
		check_command_name(token);
}
