/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 16:00:17 by shebaz            #+#    #+#             */
/*   Updated: 2024/11/19 20:21:13 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	ft_env(t_envi *env)
{
	t_envi	*tmp;

	tmp = env;
	while (tmp)
	{
		if (tmp->vale)
		{
			ft_putstr_fd(tmp->name, g_var->out_fd);
			ft_putstr_fd("=", 1);
			ft_putstr_fd(tmp->vale, g_var->out_fd);
			ft_putstr_fd("\n", 1);
		}
		tmp = tmp->next;
	}
	return (0);
}
