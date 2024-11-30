/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 00:03:12 by shebaz            #+#    #+#             */
/*   Updated: 2024/11/30 11:45:15 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

void	in_file_prep(char *path, int is_builtin, int **arr, int *size)
{
	int	fd;

	fd = open(path, O_RDONLY);
	if (fd == -1)
	{
		g_var->exit_status = 1;
		g_var->red_error = 1;
		ft_putstr_fd("minishell: ", 2);
		perror(path);
		if (!is_builtin || g_var->size > 1)
			exit(1);
	}
	else
	{
		join(fd,arr, size);
		if (dup2(fd, 0) == -1)
		{
			perror("dup2");
			exit(1);
		}
		if (fd > 2)
			close(fd);
	}
}

void	in_herdoc(char *path, int builtin, int **arr, int *size)
{
	int	fd;

	fd = open(path, O_RDONLY);
	if (fd == -1)
	{
		g_var->exit_status = 1;
		g_var->fd_here_doc = 1;
		ft_putstr_fd("minishell: ", 2);
		perror(path);
		if (!builtin || g_var->size > 1)
			exit(1);
	}
	else
	{
		join(fd, arr, size);
		g_var->fd_here_doc = 1;
		if (fd > 2)
			close(fd);
	}
}

void join(int fd, int **arr, int *size)
{
	*arr = malloc(*size + 1);
	*arr[*size + 1] = fd;
	(*size)++;
}

void	out_file_prep(char *path, int is_builtin, int **arr, int *size)
{
	int	fd;

	fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (fd == -1)
	{
		g_var->exit_status = 1;
		g_var->red_error = 1;
		ft_putstr_fd("minishell: ", 2);
		perror(path);
		if (!is_builtin || g_var->size > 1)
			exit(1);
	}
	else
	{
		join(fd,arr, size);
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit(1);
		}
		if (fd > 2)
			close(fd);
	}
}

void	append_file_prep(t_cmd *token, char *path, int is_builtin, int **arr, int *size)
{
	int	fd;

	(void)token;
	fd = open(path, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd == -1)
	{
		g_var->exit_status = 1;
		g_var->red_error = 1;
		ft_putstr_fd("minishell: ", 2);
		perror(path);
		if (!is_builtin || g_var->size > 1)
			exit(1);
	}
	else
	{
		join(fd, arr, size);
		g_var->out_fd = 1;
		if (!is_builtin || g_var->size > 1)
		{
			if (dup2(fd, STDOUT_FILENO) == -1)
			{
				perror("dup2");
				close(fd);
				exit(1);
			}
			if (fd > 2)
				close(fd);
		}
		else
			g_var->out_fd = fd;
	}
}

void	append_heredoc_prep(t_cmd *cmd, int **arr, int *size)
{
	int	fd;

	fd = open(cmd->file->filename, O_RDWR, 0777);
	if (fd == -1)
	{
		write(2, "Error\n", 6);
		exit(g_var->exit_status);
	}
	join(fd,arr, size);
	dup2(fd, STDIN_FILENO);
	g_var->in_fd = fd;
	close(fd);
	unlink(cmd->file->filename);
}

int	*files_redirections(t_cmd *cmd, int builtin, int *size)
{
	t_file	*curr_red;
	int		*arr;		

	g_var->size = count_commands(cmd);
	curr_red = cmd->file;
	while (curr_red)
	{
		if (curr_red->type == 1)
		{
			// printf("check here in\n");
			out_file_prep(curr_red->filename, builtin, &arr, size);
			// printf("check here out\n");
		}
		else if (curr_red->type == 2)
			in_file_prep(curr_red->filename, builtin, &arr, size);
		else if (curr_red->type == 3)
		{
			append_heredoc_prep(cmd, &arr, size);
			unlink(cmd->file->filename);
		}
		else if (curr_red->type == 4)
			append_file_prep(cmd, curr_red->filename, builtin, &arr, size);
		curr_red = curr_red->next;
	}
	return (arr);
}

void	lista_add_front(t_alst **lst, t_alst *new)
{
	if (!new)
		return ;
	if (*lst)
		new->next = *lst;
	*lst = new;
}

t_alst	*lista_new(void *content)
{
	t_alst	*list;

	list = malloc(sizeof(t_alst));
	if (!list)
		return (NULL);
	list->content = content;
	list->next = NULL;
	return (list);
}

void	validate_cmd(t_cmd *token)
{
	if (ft_strchr(token->arguments[0], '/'))
		check_cmd_path(token);
	else
		check_command_name(token);
}

void	ft_free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}
