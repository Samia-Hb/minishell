/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check-cmd_path.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:36:17 by szeroual          #+#    #+#             */
/*   Updated: 2024/11/24 17:23:38 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	check_path(char *path, int is_builtin)
{
	char		*folders;
	struct stat	statbuf;
	int			i;

	if (ft_strchr(path, '/') == 0)
		return (1);
	i = ft_strlen(path);
	while (i != 0 && path[i] != '/')
		i--;
	folders = allocate_folders(path, i);
	if (stat(folders, &statbuf) == -1)
		return (handle_stat_error(path, is_builtin));
	return (1);
}
