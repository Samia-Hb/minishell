/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 09:02:55 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/20 09:02:56 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

static long	validate_and_convert_exit_code(char *arg)
{
	long	exit_code;
	char	*trimmed;

	trimmed = ft_strtrim(arg, " \t\n\v\f\r");
	if (!is_numeric(trimmed) || ft_strcmp(trimmed, "") == 0)
	{
		free(trimmed);
		exit(grr(arg));
	}
	exit_code = atoi_long(trimmed);
	free(trimmed);
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
	ft_putstr_fd("exit\n", 2);
	if (i == 1)
		exit(g_var->status);
	exit_code = validate_and_convert_exit_code(args[1]);
	if (i > 2)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		g_var->exit_status = 1;
		return ;
	}
	exit((int)exit_code);
}
