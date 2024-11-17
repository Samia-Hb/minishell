/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szeroual <szeroual@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-11-17 08:54:14 by szeroual          #+#    #+#             */
/*   Updated: 2024-11-17 08:54:14 by szeroual         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../externel_folder/libftt/libft.h"
#include "../../../minishell.h"

int	ft_pwd(t_envi *env)
{
	char buff[1024];
	char *pwd = getcwd(buff, sizeof(buff));
	if (!pwd)
	{
		perror("pwd; failed");
		return (1);
	}
	printf("%s\n", pwd);
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