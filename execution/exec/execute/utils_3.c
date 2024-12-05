/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szeroual <szeroual@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 13:42:07 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/05 23:42:09 by szeroual         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	check_builtin(t_cmd *cmd)
{
	if (!cmd->arguments || !cmd->arguments[0])
		return (-1);
	if (!ft_strcmp(cmd->arguments[0], "cd"))
		return (1);
	else if (!ft_strcmp(cmd->arguments[0], "echo"))
		return (2);
	else if (!ft_strcmp(cmd->arguments[0], "env"))
		return (3);
	else if (!ft_strcmp(cmd->arguments[0], "exit"))
		return (4);
	else if (!ft_strcmp(cmd->arguments[0], "export"))
		return (5);
	else if (!ft_strcmp(cmd->arguments[0], "pwd"))
		return (6);
	else if (!ft_strcmp(cmd->arguments[0], "unset"))
		return (7);
	return (-1);
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

	list = ft_malloc(1, sizeof(t_alst));
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

void	my_strncpy(char *dest, char *src, int n)
{
	int	i;

	i = -1;
	while (++i < n)
		dest[i] = src[i];
	dest[i] = 0;
}
