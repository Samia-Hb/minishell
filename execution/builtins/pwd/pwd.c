/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 16:00:56 by shebaz            #+#    #+#             */
/*   Updated: 2024/11/30 16:13:20 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	ft_pwd(char **args, t_envi *env)
{
	char	buff[1024];
	char	*pwd;

	if (args[1])
		return (g_var->exit_status = 1,
			printf("invalid option : %s\n", args[1]), 1);
	pwd = getcwd(buff, sizeof(buff));
	if (!pwd)
	{
		perror("pwd; failed");
		return (1);
	}
	write(1, pwd, ft_strlen(pwd));
	write(1, "\n", 1);
	if (env)
	{
		free(env->vale);
		env->vale = ft_strdup(pwd);
		if (!env->vale)
		{
			perror("Failed to duplicate pwd");
			return (1);
		}
	}
	return (0);
}
