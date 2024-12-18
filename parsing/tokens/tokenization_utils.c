/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 13:36:28 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/16 02:33:14 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	built_in_checker(const char *str)
{
	if (!ft_strcmp(str, "export") || !ft_strcmp(str, "cd") || !ft_strcmp(str,
			"pwd") || !ft_strcmp(str, "echo") || !ft_strcmp(str, "unset")
		|| !ft_strcmp(str, "env") || !ft_strcmp(str, "exit"))
		return (1);
	return (0);
}

int	get_token_type(const char *token, char c)
{
	char	*path;

	path = get_executable((char *)token);
	if (c)
	{
		if (c == '"')
			return (TOKEN_DOUBLE_QUOTED);
		return (TOKEN_SINGLE_QUOTED);
	}
	if (built_in_checker(token))
		return (TOKEN_BUILT_IN);
	if (!ft_strcmp(token, "<<"))
		return (TOKEN_REDIR_HERE_DOC);
	if (!ft_strcmp(token, ">"))
		return (TOKEN_REDIR_OUT);
	if (!ft_strcmp(token, "<"))
		return (TOKEN_REDIR_IN);
	if (!ft_strcmp(token, ">>"))
		return (TOKEN_REDIR_APPEND);
	if (!ft_strcmp(token, "|"))
		return (TOKEN_PIPE);
	if (path)
		return (TOKEN_COMMAND);
	return (TOKEN_UNKNOWN);
}

char	*char_to_string(char c, char c2)
{
	char	*string;

	if (!c2)
	{
		string = ft_malloc(2, sizeof(char));
		if (!string)
		{
			ft_putstr_fd("Memory allocation failed\n", 2);
			exit(EXIT_FAILURE);
		}
		string[0] = c;
		string[1] = '\0';
		return (string);
	}
	string = ft_malloc(3, sizeof(char));
	if (!string)
	{
		ft_putstr_fd("Memory allocation failed\n", 2);
		exit(EXIT_FAILURE);
	}
	string[0] = c;
	string[1] = c;
	string[2] = '\0';
	return (string);
}

int	ft_strschr(char *string, char *delimiteur, int *l)
{
	int		i;
	char	**splited;

	splited = ft_split(string, '\n');
	i = 0;
	if (!splited)
	{
		ft_putstr_fd("Error\n", 2);
		return (0);
	}
	while (splited[i])
	{
		*l += ft_strlen(splited[i]);
		if (!ft_strcmp(splited[i], delimiteur))
			return (1);
		i++;
	}
	return (0);
}

char	*handle_dollar(char *str)
{
	int		i;
	char	*word;

	i = 0;
	while (str[i] && !isspace(str[i]))
		i++;
	word = ft_malloc(i + 1, sizeof(char));
	if (!word)
		return (NULL);
	word = strncpy(word, str, i);
	return (word);
}
