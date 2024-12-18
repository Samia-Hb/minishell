/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szeroual <szeroual@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 10:19:15 by szeroual          #+#    #+#             */
/*   Updated: 2024/12/18 10:19:18 by szeroual         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	ft_pwd(char **args)
{
	char	buff[1024];
	char	*pwd;

	(void)args;
	pwd = getcwd(buff, sizeof(buff));
	if (!pwd)
	{
		perror("pwd; failed");
		return (1);
	}
	write(1, pwd, ft_strlen(pwd));
	write(1, "\n", 1);
	return (0);
}
