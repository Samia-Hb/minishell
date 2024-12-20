/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 10:18:32 by szeroual          #+#    #+#             */
/*   Updated: 2024/12/20 09:02:42 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	ft_env(t_envi *env)
{
	t_envi	*tmp;

	tmp = env;
	while (tmp)
	{
		if (tmp && tmp->vale)
		{
			ft_putstr_fd(tmp->name, 1);
			ft_putstr_fd("=", 1);
			ft_putstr_fd(tmp->vale, 1);
			ft_putstr_fd("\n", 1);
		}
		tmp = tmp->next;
	}
	return (0);
}
