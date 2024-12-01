/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 13:07:11 by szeroual          #+#    #+#             */
/*   Updated: 2024/12/01 17:37:31 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

// void	handle_signals(int mode)
// {
// 	if (mode == 0)
// 	{
// 		signal(SIGINT, SIG_IGN);
// 		signal(SIGQUIT, SIG_IGN);
// 	}
// 	else
// 	{
// 		signal(SIGQUIT, SIG_IGN);
// 	}
// }

// void	sig_wait(t_cmd *token)
// {
// 	int		status;
// 	t_cmd	*current;

// 	handle_signals(0);
// 	current = token;
// 	status = 0;
// 	while (current)
// 	{
// 		waitpid(current->pid, &status, 0);
// 		current = current->next;
// 	}
// 	if (g_var->exit_status)
// 		return ;
// 	g_var->exit_status = 128 + WTERMSIG(status);
// 	if (WTERMSIG(status) + 128 == 130)
// 	{
// 		g_var->exit_status = 128 + WTERMSIG(status);
// 		write(1, "\n", 1);
// 	}
// 	else if (WTERMSIG(status) + 128 == 131)
// 	{
// 		g_var->exit_status = 128 + WTERMSIG(status);
// 		write(1, "Quit\n", 6);
// 	}
// 	else
// 		g_var->exit_status = WEXITSTATUS(status);
// 	handle_signals(1);
// }

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
