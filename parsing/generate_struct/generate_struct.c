/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generate_struct.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 19:05:46 by shebaz            #+#    #+#             */
/*   Updated: 2024/11/02 16:57:43 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	heredoc_process(t_cmd **node, t_file **head ,Token **tokens)
{
	int		fd;
	char	*processed_del;
	char	*line;

	(*tokens) = (*tokens)->next;
	processed_del = process_delimiter((*tokens)->value);  
	fd = open((*tokens)->value, O_CREAT | O_TRUNC | O_RDWR, 0777);
	while(1)
	{
		line = readline("heredoc > ");
		if (!ft_strncmp(line, processed_del, ft_strlen(processed_del)))
			break ;
		if (!is_quoted((*tokens)->value))
			line = parse_line(line);
		write(fd, line, ft_strlen(line));
	}
	(*node)->file = malloc(sizeof(t_file));
	(*node)->file->type = RE_HEREDOC;
	(*node)->file->filename = ft_strdup((*tokens)->value);
	(*tokens) = (*tokens)->next;
	push_t_file(head, (*node)->file);
	close(fd);
	free(line);
	free(processed_del);
}

void	red_process(Token **tokens, t_cmd **node, int *i)
{
	t_file	*head;

	head = malloc(sizeof(t_file));
	head = NULL;
	while ((*tokens) && (*tokens)->type != TOKEN_PIPE)
	{
		if ((*tokens)->type == TOKEN_REDIR_HERE_DOC)
			heredoc_process(node, &head, tokens);
		if (is_red(*tokens) && (*tokens)->type != TOKEN_REDIR_HERE_DOC)
		{			
			(*node)->file = malloc(sizeof(t_file));
			(*node)->file->type = get_red_type(*tokens);
			if ((*tokens)->next)
			{
				*tokens = (*tokens)->next;
				(*node)->file->filename = ft_strdup((*tokens)->value);
				(*node)->file->next = NULL;
				*tokens = (*tokens)->next;
				push_t_file(&head, (*node)->file);
			}
		}
		while ((*tokens) && !is_red(*tokens))
		{
			(*node)->arguments[(*i)++] = ft_strdup((*tokens)->value);
			(*tokens) = (*tokens)->next;
		}
	}
	(*node)->file = head;
}

void	print_node(t_cmd *node)
{
	int	i;

	while (node)
	{
		if (node->arguments)
		{
			i = 0;
			while (node->arguments[i])
			{
				printf("arg[%d] = %s\n", i, node->arguments[i]);
				i++;
			}
		}
		if (node->file)
		{
			while (node->file)
			{
				node->file = node->file->next;
				node = node->next;
			}
		}
	}
}
int is_quoted(char *input)
{
	int i;

	i = 0;
	while(input[i])
	{
		if(input[i] == '"' || input[i] == '\'')
			return (1);
		i++;
	}
	return (0);
}

char *parse_line(char *input)
{
	char	*result;

	result = expand_non_operator(input);
	return (result);
}

char	*process_delimiter(char *tmp)
{
	int		i;
	int		j;
	char	c;
	char	*result;
	char	*hp;
	char	*temp;

	i = 0;
	result = ft_strdup("");
	while (tmp[i])
	{
		j = i;
		if (tmp[i] == '"' || tmp[i] == '\'')
		{
			c = tmp[i++];
			while (tmp[i] && tmp[i] != c)
				i++;
			if (tmp[i] == c)
				i++;
			hp = ft_strndup(tmp + j + 1, i - j - 2);
		}
		else
		{
			while (tmp[i] && tmp[i] != '"' && tmp[i] != '\'')
				i++;
			hp = ft_strndup(tmp + j, i - j);
		}
		temp = result;
		result = ft_strjoin(result, hp);
		free(temp);
		free(hp);
	}
	return (result);
}

void	create_node_arguments(t_cmd **node, Token **tokens)
{
	int	i;
	int	j;

	i = 0;
	(*node)->arguments = malloc((nbr_argument(*tokens) + 1) * sizeof(char *));
	if ((*tokens)->expanded_value)
	{
		(*node)->arguments[i++] = ft_strdup((*tokens)->expanded_value[0]);
		(*tokens) = (*tokens)->next;
	}
	while (*tokens && (*tokens)->type != TOKEN_PIPE)
	{
		j = 0;
		// if ((*tokens)->type == TOKEN_REDIR_HERE_DOC)
		// 	heredoc_process(node, tokens);
		if (*tokens && !is_red(*tokens))
		{
			while ((*tokens)->expanded_value[j])
				(*node)->arguments[i++] = ft_strdup((*tokens)->expanded_value[j++]);
			(*tokens) = (*tokens)->next;
		}
		else
			red_process(tokens, node, &i);
	}
	if (*tokens && (*tokens)->type == TOKEN_PIPE)
		(*tokens) = (*tokens)->next;
	(*node)->arguments[i] = NULL;
}

t_cmd	*analyse_tokens(Token **tokens)
{
	t_cmd	*head;
	t_cmd	*node;

	head = NULL;
	while (*tokens)
	{
		node = malloc(sizeof(t_cmd));
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
	}
	return (head);
}
