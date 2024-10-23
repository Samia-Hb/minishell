/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 19:03:34 by shebaz            #+#    #+#             */
/*   Updated: 2024/10/23 22:16:24 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	clean_arr(char **arr)
{
	int	i;

	i = 0;
	if (arr)
	{
		while (arr[i])
		{
			free(arr[i]);
			i++;
		}
		free(arr);
	}
}

char	*find_command_path(char **dir, char *command) // work
{
	char	*full_path;
	int		i;

	i = 0;
	while (dir[i])
	{
		full_path = malloc(ft_strlen(dir[i]) + ft_strlen(command) + 2);
		if (!full_path)
		{
			perror("malloc");
			exit(1);
		}
		strcpy(full_path, dir[i]);
		strcat(full_path, "/");
		strcat(full_path, command);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

void	check_single_quote(char **str)
{
	if (*str[0] == 39 && *str[ft_strlen(*str) - 1] == 39)
		*str = ft_strtrim(*str, "'");
}

char	*get_executable(char *command)
{
	char	*command_path;
	char	**dir;
	char	**arr;
	char	*path;

	path = getenv("PATH");
	arr = ft_split(command, ' ');
	check_single_quote(&arr[0]);
	if (access(arr[0], X_OK) == 0)
	{
		command_path = ft_strdup(arr[0]);
		clean_arr(arr);
		return (command_path);
	}
	dir = ft_split(path, ':');
	command_path = find_command_path(dir, command);
	clean_arr(dir);
	clean_arr(arr);
	return (command_path);
}
