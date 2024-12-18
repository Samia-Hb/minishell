/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szeroual <szeroual@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 10:20:31 by szeroual          #+#    #+#             */
/*   Updated: 2024/12/18 10:20:38 by szeroual         ###   ########.fr       */
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
		close_open_file();
		exit(127);
	}
	g_var->exit_status = 126;
	close_open_file();
	exit(126);
}

void	close_file_descriptors(void)
{
	if (g_var->pre_pipe_infd > 2)
		close(g_var->pre_pipe_infd);
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

void	exec_builtin(int btn, t_cmd *cmd, t_envi *envi)
{
	if (btn == 1)
		ft_cd(cmd->arguments, envi);
	else if (btn == 2)
		ft_echo(cmd->arguments);
	else if (btn == 3)
		ft_env(envi);
	else if (btn == 4)
		ft_exit(cmd->arguments);
	else if (btn == 5)
		ft_export(cmd->arguments);
	else if (btn == 6)
		ft_pwd(cmd->arguments);
	else if (btn == 7)
		ft_unset(cmd->arguments, &envi);
	if (g_var->out_fd > 2)
		close(g_var->out_fd);
	g_var->out_fd = 1;
}
