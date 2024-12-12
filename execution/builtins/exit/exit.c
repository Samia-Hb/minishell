#include "../../../minishell.h"
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

static int	grr(char *str)
{
    ft_putstr_fd("minishell: exit: ", 2);
    ft_putstr_fd(str, 2);
    ft_putstr_fd(": numeric argument required\n", 2);
    return (2);
}

static int	is_numeric(char *str)
{
    int i = 0;
    if (str[i] == '-' || str[i] == '+')
        i++;
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return (0);
        i++;
    }
    return (1);
}

int	ft_exit(char **args)
{
    int i = 0;
    while (args[i])
        i++;
    if (i == 1)
        exit(0);
    if (!is_numeric(args[1]))
        exit(grr(args[1]));
    if (i > 2)
    {
        ft_putstr_fd("minishell: exit: too many arguments\n", 2);
        return (1);
    }
    exit(atoi(args[1]));
}