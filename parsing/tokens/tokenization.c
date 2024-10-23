/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 13:36:37 by shebaz            #+#    #+#             */
/*   Updated: 2024/10/23 20:54:12 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*heredoc_token(char *input, int l)
{
	char	*full_token;
	int		i;

	i = 0;
	full_token = malloc(l * sizeof(char));
	if (!full_token)
		return (NULL);
	while (i < l)
	{
		full_token[i] = input[i];
		i++;
	}
	return (full_token);
}

char	*read_from_stdin(char *result, char *buffer, int *n)
{
	result = ft_strjoin(result, buffer);
	write(0, "> ", 2);
	buffer = get_next_line(STDIN_FILENO);
	*n = *n + ft_strlen(result);
	return (result);
}

char	*handle_heredoc(char *input, int *n) // until I find how it will be done
{
	int		i;
	int		k;
	char	*delimiter;
	char	*buffer;
	char	*result;

	*n = 0;
	i = 0;
	buffer = ft_strdup(" ");
	result = ft_strdup("");
	while (input[i] && (input[i] == '<' || input[i] == ' ' || input[i] == '\t'))
		i++;
	k = i;
	while (input[k] != ' ' && input[k] != '\t' && input[k] != '\n')
		k++;
	delimiter = ft_strndup(input + i, k - i);
	result = ft_strjoin(result, delimiter);
	k = i + ft_strlen(delimiter);
	while (input[++k] != '\n')
		result = ft_strjoin(result, char_to_string(input[i], 0));
	while (ft_strncmp(buffer, delimiter, ft_strlen(delimiter)))
		result = read_from_stdin(result, buffer, n);
	return (result);
}

void	handle_operators(char *input, Token **tokens, int *j, int *k)
{
	char	*str1;
	char	*str2;
	int		i;

	i = 0;
	str2 = NULL;
	str1 = char_to_string(input[i], 0);
	if (input[i] == '>' || input[i] == '<' || input[i] == '|')
		add_token(tokens, get_token_type(str1, 0), str1, j);
	else if (input[i] == '>' && input[i + 1] && input[i + 1] == '>')
	{
		str2 = char_to_string(input[i], 1);
		add_token(tokens, get_token_type(str1, 0), str2, j);
	}
	if (str1)
		free(str1);
	if (str2)
		free(str2);
	*k = 0;
}

void	handle_word(char *input, Token **tokens, int *j, int *k)
{
	int		token_type;
	char	*word;
	int		i;
	char	*str;

	i = 0;
	while (input[i] && !ft_is_separator(input[i]))
	{
		if (input[i] == '"' || input[i] == '\'')
		{
			str = handle_quote(input + i);
			i += ft_strlen(str);
		}
		i++;
	}
	word = ft_strndup(input, i);
	token_type = get_token_type(word, 0);
	if ((token_type == 14 || token_type == 11) && *k == 0)
	{
		add_token(tokens, token_type, word, j);
		*k = 1;
	}
	else
		add_token(tokens, TOKEN_ARGUMENT, word, j);
	free(word);
}

int	is_ope(char *input)
{
	int	i;

	i = 0;
	if (input[i] == '>' || input[i] == '<' || input[i] == '|'
		|| (input[i] == '>' && input[i + 1] && input[i + 1] == '>'))
		return (1);
	return (0);
}

void	processing_data(Token **tokens, char *input, int *i, int *k)
{
	char	*word;

	word = NULL;
	if (input[*i] == ' ' || input[*i] == '\t')
		(*i)++;
	else if ((input[*i] == '\\' && (input[(*i) + 1] == '"'
				|| input[*i] == '\'')) || (input[*i] == '"'
			|| input[*i] == '\''))
	{
		word = handle_quote(input + (*i));
		add_token(tokens, get_token_type(word, input[*i]), word, i);
	}
	else if (input[*i] == '<' && input[(*i) + 1] && input[(*i) + 1] == '<')
	{
		word = handle_heredoc(input + (*i), k);
		add_token(tokens, TOKEN_REDIR_HERE_DOC, word, i);
	}
	else if (is_ope(input + (*i)))
		handle_operators(input + (*i), tokens, i, k);
	else
		handle_word(input + (*i), tokens, i, k);
	if (word)
		free(word);
}

Token	**tokenize(char *input)
{
	Token	**tokens;
	int		i;
	int		k;

	i = 0;
	k = 0;
	tokens = (Token **)malloc(sizeof(Token *));
	if (!tokens)
	{
		printf("Error: Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}
	*tokens = NULL;
	while (input[i])
	{
		processing_data(tokens, input, &i, &k);
	}
	return (tokens);
}
