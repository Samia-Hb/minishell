/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 13:07:11 by szeroual          #+#    #+#             */
/*   Updated: 2024/11/21 15:49:25 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

void	handle_signals(int mode)
{
	if (mode == 0)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
	else
	{
		signal(SIGQUIT, SIG_IGN);
	}
}

void	sig_wait(t_cmd *token)
{
	int			status;
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


void	print_perror(char *str, int exitt)
{
	ft_putstr_fd("minishell: ", 2);
	perror(str);
	g_var->exit_status = 127;
	if (exitt)
	{
		//free_hdfiles();
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
        {
            // ft_free_array(dirs);
            return (NULL);
        }
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
			// free_hdfiles();
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

int	handle_stat_error(char *path, int is_builtin)
{
	ft_putstr_fd("minishell: ", 2);
	perror(path);
	if (is_builtin)
	{
		g_var->red_error = 1;
		g_var->exit_status = 1;
		return (0);
	}
	else
	{
		exit(1);
	}
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
			// free_hdfiles();
			exit(127);
		}
		else if (cmd && cmd[0] != '$')
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd, 2);
			ft_putstr_fd(": permission denied\n", 2);
			// free_hdfiles();
			exit(126);
		}
		return (NULL);
	}
	else
		return (cmd_path);
}

// void	append_heredoc_prep(t_cmd *cmd)
// {
// 	int	fd;

// 	fd = open(cmd->file->filename, O_RDWR, 0777);
// 	dup2(fd, STDIN_FILENO);
// 	g_var->in_fd = fd;
// }

// void	validate_cmd(t_cmd *cmd)
// {
// 	if (ft_strchr(cmd->arguments[0], '/'))
// 		check_cmd_path(cmd);
// 	else
// 		check_command_name(cmd);
// }

// int	count_commands(t_cmd *cmd)
// {
// 	int		count;
// 	t_cmd	*current;

// 	count = 0;
// 	current = cmd;
// 	while (current)
// 	{
// 		count++;
// 		current = current->next;
// 	}
// 	return (count);
// }

// char	*allocate_folders(char *path, int i)
// {
// 	char	*folders;

// 	folders = malloc(i + 2);
// 	if (!folders)
// 	{
// 		perror("malloc failed");
// 		exit(1);
// 	}
// 	my_strncpy(folders, path, i + 1);
// 	return (folders);
// }

// void	my_strncpy(char *dest, const char *src, size_t n)
// {
// 	size_t	i;

// 	i = 0;
// 	while (i < n && src[i] != '\0')
// 	{
// 		dest[i] = src[i];
// 		i++;
// 	}
// 	while (i < n)
// 	{
// 		dest[i] = '\0';
// 		i++;
// 	}
// }
