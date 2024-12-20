/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generate_struct.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 19:05:46 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/20 09:00:04 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	child_proces(char *token, char *processed_del, int fd)
{
	char	*line;

	line = NULL;
	signal(SIGINT, ctrl_c);
	while (1)
	{
		line = readline("heredoc > ");
		if (!line)
			break ;
		if (!ft_strcmp(line, processed_del))
			break ;
		if (!is_quoted(token))
			line = parse_line(line);
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
	}
	if (!line)
		printf("minishell : warning: heredoc delimited by EOF \n");
	close(fd);
	close(STDERR_FILENO);
	close(STDOUT_FILENO);
	close(STDIN_FILENO);
	ft_free_envp(g_var->envp);
	clean_gc();
	exit(0);
}

void	heredoc_process(t_cmd **node, t_file **head, t_token **tokens)
{
	int		fd;
	char	*processed_del;
	int		pid;
	int		status;

	status = 0;
	(*tokens) = (*tokens)->next;
	processed_del = process_delimiter((*tokens)->value);
	(*node)->file = ft_malloc(1, sizeof(t_file));
	(*node)->file->type = RE_HEREDOC;
	(*node)->file->filename = generate_name(&g_var->i);
	fd = open((*node)->file->filename, O_CREAT | O_TRUNC | O_RDWR, 0777);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (!pid)
		child_proces((*tokens)->value, processed_del, fd);
	else
		waitpid(pid, &status, 0);
	if (WEXITSTATUS(status))
		g_var->stop = 1;
	push_t_file(head, (*node)->file);
	(*tokens) = (*tokens)->next;
	close(fd);
	exit_status(status, (*node)->file->filename);
}

void	red_process(t_token **tokens, t_cmd **node, int *i)
{
	t_file	*head;

	head = ft_malloc(1, sizeof(t_file));
	head = NULL;
	while ((*tokens) && (*tokens)->type != TOKEN_PIPE)
	{
		if ((*tokens)->type == TOKEN_REDIR_HERE_DOC)
		{
			heredoc_process(node, &head, tokens);
			if (g_var->stop == 1)
				break ;
		}
		if ((*tokens) && is_red(*tokens)
			&& (*tokens)->type != TOKEN_REDIR_HERE_DOC)
			fill_up_node(node, tokens, &head);
		while ((*tokens) && !is_red(*tokens) && (*tokens)->type != TOKEN_PIPE)
		{
			(*node)->arguments[(*i)++] = ft_strdup((*tokens)->value);
			(*tokens) = (*tokens)->next;
		}
	}
	(*node)->file = head;
}

void	create_node_arguments(t_cmd **node, t_token **tokens)
{
	int	i;
	int	j;

	i = 0;
	(*node)->arguments = ft_malloc(nbr_argument(*tokens) + 1, sizeof(char *));
	while (*tokens && (*tokens)->type != TOKEN_PIPE)
	{
		j = 0;
		if (*tokens && !is_red(*tokens))
		{
			while ((*tokens)->expanded_value[j])
				(*node)->arguments[i++]
					= ft_strdup((*tokens)->expanded_value[j++]);
			(*tokens) = (*tokens)->next;
		}
		else
		{
			red_process(tokens, node, &i);
			if (g_var->stop == 1)
				break ;
		}
	}
	go_to_next(tokens);
	(*node)->arguments[i] = NULL;
}

t_cmd	*analyse_tokens(t_token **tokens)
{
	t_cmd	*head;
	t_cmd	*node;

	head = NULL;
	while (*tokens)
	{
		node = ft_malloc(1, sizeof(t_cmd));
		node->arguments = NULL;
		node->file = NULL;
		if ((*tokens)->type == TOKEN_COMMAND
			|| (*tokens)->type == TOKEN_BUILT_IN)
			create_node_arguments(&node, tokens);
		else if ((*tokens)->type != TOKEN_COMMAND
			|| (*tokens)->type != TOKEN_BUILT_IN)
			create_node_arguments(&node, tokens);
		else
			(*tokens) = (*tokens)->next;
		push_back(&head, node);
		if (g_var->stop)
			break ;
	}
	return (head);
}
