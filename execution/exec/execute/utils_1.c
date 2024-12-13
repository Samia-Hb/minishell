/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 13:36:31 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/13 19:01:34 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	get_var_index(char *key)
{
	int	j;
	int	i;

	j = -1;
	i = -1;
	while (g_var->en[++j])
	{
		if (ft_strncmp(g_var->en[j], key, ft_strlen(key)) == 0)
		{
			i = j;
			break ;
		}
	}
	return (i);
}

char	*allocate_folders(char *path, int i)
{
	char	*folders;

	folders = ft_malloc(i + 2, sizeof(char));
	if (!folders)
	{
		perror("");
		exit(1);
	}
	lista_add_front(g_var->alist, lista_new(folders));
	my_strncpy(folders, path, i + 1);
	return (folders);
}

int	handle_stat_error(char *path, int is_builtin)
{
	ft_putstr_fd("minishell: ", 2);
	perror(path);
	if (is_builtin)
	{
		g_var->exit_status = 1;
		return (0);
	}
	else
		exit(1);
}

int	count_commands(t_cmd *cmd)
{
	int		count;
	t_cmd	*current;

	count = 0;
	current = cmd;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

char	*put_cmd_status(int status, char *cmd_path, char *cmd)
{
	if (status)
	{
		if (status == 1 && cmd && cmd[0] != '$')
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd, 2);
			ft_putstr_fd(": command not found\n", 2);
			exit(127);
		}
		else if (cmd && cmd[0] != '$')
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd, 2);
			ft_putstr_fd(": permission denied\n", 2);
			exit(126);
		}
		return (NULL);
	}
	else
		return (cmd_path);
}
