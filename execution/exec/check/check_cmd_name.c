/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmd_name.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanaa <sanaa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:37:38 by szeroual          #+#    #+#             */
/*   Updated: 2024/12/14 10:59:46 by sanaa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

char	*construct_full_path(char *dir, char *cmd)
{
	char	*full_path;

	full_path = ft_malloc(strlen(dir) + strlen(cmd) + 2, sizeof(char));
	if (!full_path)
	{
		perror("Calloc failed");
		return (NULL);
	}
	strcpy(full_path, dir);
	strcat(full_path, "/");
	strcat(full_path, cmd);
	return (full_path);
}

void	handle_command_not_found(t_cmd *cmd)
{
	ft_putstr_fd("minishell: command not found: ", 2);
	ft_putstr_fd(cmd->arguments[0], 2);
	ft_putstr_fd("\n", 2);
	g_var->exit_status = 127;
}

void	search_command_in_paths(t_cmd *cmd, char **path_dirs)
{
	char	*full_path;
	int		i;

	i = 0;
	while (path_dirs[i])
	{
		full_path = construct_full_path(path_dirs[i], cmd->arguments[0]);
		if (!full_path)
			return ;
		if (!access(full_path, X_OK))
		{
			cmd->cmd_path = full_path;
			return ;
		}
		i++;
	}
	handle_command_not_found(cmd);
}

void	check_command_name(t_cmd *cmd)
{
	char	*path_env;
	char	**path_dirs;

	path_env = ft_getenv("PATH");
	if (!path_env)
	{
		handle_command_not_found(cmd);
		return ;
	}
	path_dirs = ft_split(path_env, ':');
	if (!path_dirs)
	{
		perror("ft_split failed");
		return ;
	}
	search_command_in_paths(cmd, path_dirs);
}
