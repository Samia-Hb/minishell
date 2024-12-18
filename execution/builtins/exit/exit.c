/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanaa <sanaa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 01:00:05 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/18 23:48:54 by sanaa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

static long	validate_and_convert_exit_code(char *arg)
{
	long	exit_code;

	if (!is_numeric(arg) || ft_strcmp(arg, "") == 0)
	{
		printf("in the convert function\n");
		exit(grr(arg));
	}
	exit_code = atoi_long(arg);
	if (exit_code == LONG_MAX || exit_code == LONG_MIN)
	{
		ft_putstr_fd("exit\n", 2);
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit(2);
	}
	return (exit_code);
}

void	ft_exit(char **args)
{
	long	exit_code;
	int		i;

	i = 0;
	while (args[i])
		i++;
	if (i == 1)
		exit(0);
	exit_code = validate_and_convert_exit_code(args[1]);
	if (i > 2)
	{
		ft_putstr_fd("exit\n", 2);
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		g_var->exit_status = 1;
		return ;
	}
	exit((int)exit_code);
}
