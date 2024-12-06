/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanaa <sanaa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 21:45:07 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/06 11:51:39 by sanaa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

static int	grr(char *str)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	return (2);
}

static int	check_long(int sign, long long res, char *str, int i)
{
	if (sign == 1 && res > (LLONG_MAX - (str[i] - '0')) / 10)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(str, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		return (1);
	}
	else if (sign == -1 && (-res) < (LLONG_MIN + (str[i] - '0')) / 10)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(str, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		return (1);
	}
	return (0);
}

static int	normii(char *str, int i, int res, int sign)
{
	if (str[i] != '\0')
		return (grr(str));
	return ((res * sign));
}

static int	atoi_exit(char *str)
{
	long long	res;
	int			sign;
	int			i;

	i = 0;
	res = 0;
	sign = 1;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-')
		sign *= -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (grr(str));
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		if (check_long(sign, res, str, i))
			return (2);
		res = res * 10 + str[i] - '0';
		i++;
	}
	return (normii(str, i, res, sign));
}

void	ft_exit(char **av)
{
	int	exit_s;

	if (!av[1])
		exit_s = g_var->exit_status;
	else if (av[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return ;
	}
	else
		exit_s = atoi_exit(av[1]);
	if (g_var->interactive != 1)
		printf("exit\n");
	exit((unsigned char)exit_s);
}
