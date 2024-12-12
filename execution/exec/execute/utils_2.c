/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 13:37:08 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/12 17:45:06 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

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
			print_perror(token->arguments[0], 0);
	}
	else
		print_perror(token->arguments[0], 1);
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
