/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szeroual <szeroual@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 13:07:11 by szeroual          #+#    #+#             */
/*   Updated: 2024/12/05 23:42:13 by szeroual         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

void	print_perror(char *str, int exitt)
{
	ft_putstr_fd("minishell: ", 2);
	perror(str);
	if (exitt)
	{
		g_var->exit_status = 127;
		exit(127);
	}
	g_var->exit_status = 126;
	exit(126);
}

int	path_status(char *cmd_path)
{
	int	status;

	status = 1;
	if (access(cmd_path, F_OK))
		status = 1;
	else if (access(cmd_path, X_OK))
		status = 2;
	else
		status = 0;
	if (status)
	{
		free(cmd_path);
		cmd_path = NULL;
	}
	return (status);
}

char	*get_cmd_path(char *cmd, char **dirs)
{
	char	*cmd_path;
	char	*temp;
	int		i;
	int		status;

	i = -1;
	status = 1;
	cmd_path = NULL;
	temp = NULL;
	while (dirs[++i] && cmd && cmd[0] && status)
	{
		temp = ft_strjoin(dirs[i], "/");
		if (!temp)
			return (NULL);
		cmd_path = ft_strjoin(temp, cmd);
		if (!cmd_path)
			return (NULL);
		status = path_status(cmd_path);
		if (status)
		{
			free(cmd_path);
			cmd_path = NULL;
		}
	}
	return (put_cmd_status(status, cmd_path, cmd));
}
