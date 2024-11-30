/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 13:07:11 by szeroual          #+#    #+#             */
/*   Updated: 2024/11/30 13:38:49 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

void	print_perror(char *str, int exitt)
{
	ft_putstr_fd("minishell: ", 2);
	perror(str);
	g_var->exit_status = 127;
	if (exitt)
	{
		exit(127);
	}
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
		free(temp);
		if (!cmd_path)
		{
			return (NULL);
		}
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
	}
	return (put_cmd_status(status, cmd_path, cmd));
}

void	my_strncpy(char *dest, char *src, int n)
{
	int	i;

	i = -1;
	while (++i < n)
		dest[i] = src[i];
	dest[i] = 0;
}

void	check_cmd_path(t_cmd *token)
{
	struct stat	statbuf;
	DIR			*dir;

	if (stat(token->arguments[0], &statbuf) == 0)
	{
		dir = opendir(token->arguments[0]);
		if (dir == NULL && !access(token->arguments[0], F_OK)
			&& !access(token->arguments[0], X_OK))
			token->cmd_path = token->arguments[0];
		else if (dir != NULL)
		{
			closedir(dir);
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(token->arguments[0], 2);
			ft_putstr_fd(" is a directory\n", 2);
			exit(126);
		}
		else
			print_perror(token->arguments[0], 1);
	}
	else
		print_perror(token->arguments[0], 1);
}

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
