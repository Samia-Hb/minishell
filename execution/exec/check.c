
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szeroual <szeroual@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-11-16 13:07:45 by szeroual          #+#    #+#             */
/*   Updated: 2024-11-16 13:07:45 by szeroual         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	check_cmd_path(t_cmd *cmd)
{
	struct stat	statbuf;

	if (stat(cmd->arguments[0], &statbuf) == 0)
	{
		if (S_ISREG(statbuf.st_mode) && (statbuf.st_mode & S_IXUSR))
		{
			cmd->cmd_path = cmd->arguments[0];
		}
		else
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->arguments[0], 2);
			ft_putstr_fd(": Permission denied\n", 2);
			g_var->exit_status = 1;
		}
	}
	else
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->arguments[0], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		g_var->exit_status = 1;
	}
}

int	check_path(char *path, int builtin)
{
	struct stat	statbuf;
	int			i;
	char		*folders;
	int			status;

	if (builtin)
		return (1);
	if (!ft_strchr(path, '/'))
		return (1);
	i = ft_strlen(path);
	while (i != 0 && path[i] != '/')
		i--;
	folders = allocate_folders(path, i);
	status = (stat(folders, &statbuf) != -1);
	free(folders);
	return (status);
}

void	check_command_name(t_cmd *cmd)
{
	char		*path_env;
	char		**path_dirs;
	struct stat	statbuf;
	int			i;
	char		*full_path;

	path_env = getenv("PATH");
	path_dirs = ft_split(path_env, ':');
	i = 0;
	while (path_dirs[i])
	{
		full_path = malloc(strlen(path_dirs[i]) + strlen(cmd->arguments[0])
				+ 2);
		if (!full_path)
		{
			perror("malloc failed");
			return ;
		}
		strcpy(full_path, path_dirs[i]);
		strcat(full_path, "/");
		strcat(full_path, cmd->arguments[0]);
		if (stat(full_path, &statbuf) == 0 && S_ISREG(statbuf.st_mode)
			&& (statbuf.st_mode & S_IXUSR))
		{
			cmd->cmd_path = full_path;
			free(path_dirs);
			return ;
		}
		free(full_path);
		i++;
	}
	ft_putstr_fd("minishell: command not found: ", 2);
	ft_putstr_fd(cmd->arguments[0], 2);
	ft_putstr_fd("\n", 2);
	g_var->exit_status = 127;
	free(path_dirs);
}

int	check_file_errors(char *path, int builtin)
{
	if (path && (path[0] == '$' || (path[0] == '"' && path[1] == '$')))
	{
		g_var->red_error = 1;
		g_var->exit_status = 1;
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(" ambiguous redirect\n", 2);
		if (builtin)
			return (1);
		else
			exit(1);
	}
	return (0);
}
