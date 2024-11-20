/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmd_name.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szeroual <szeroual@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-11-20 10:37:38 by szeroual          #+#    #+#             */
/*   Updated: 2024-11-20 10:37:38 by szeroual         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

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