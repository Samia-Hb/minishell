/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 13:33:43 by shebaz            #+#    #+#             */
/*   Updated: 2024/11/30 13:38:37 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

void	sig_wait(t_cmd *token)
{
	int		status;
	t_cmd	*current;

	handle_signals(0);
	current = token;
	status = 0;
	while (current)
	{
		waitpid(current->pid, &status, 0);
		current = current->next;
	}
	g_var->exit_status = 128 + WTERMSIG(status);
	if (WTERMSIG(status) + 128 == 130)
	{
		g_var->exit_status = 128 + WTERMSIG(status);
		write(1, "\n", 1);
	}
	else if (WTERMSIG(status) + 128 == 131)
	{
		g_var->exit_status = 128 + WTERMSIG(status);
		write(1, "Quit\n", 6);
	}
	else
		g_var->exit_status = WEXITSTATUS(status);
	handle_signals(1);
}

char	*allocate_folders(char *path, int i)
{
	char	*folders;

	folders = malloc(i + 2);
	if (!folders)
	{
		perror("");
		exit(1);
	}
	lista_add_front(g_var->alist, lista_new(folders));
	my_strncpy(folders, path, i + 1);
	return (folders);
}
