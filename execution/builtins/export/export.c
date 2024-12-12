/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 21:41:35 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/12 01:03:08 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	check_each_element(char *str)
{
	char	*arr[2];
	char	*tmp;
	int		i;

	i = 0;
	tmp = ft_strdup(str);
	if (!ft_strlen(str))
		return (1);
	arr[0] = strtok(tmp, "=");
	if (!arr[0])
		return (1);
	while (arr[0][i])
	{
		if (is_number(str[i]))
			return (1);
		while (arr[0][i])
		{
			if ((is_special(str[i]) && str[i] != '\\' && str[i] != '$')
				|| str[0] == '$' || str[0] == '=' || str[i] == '*'
				|| str[i] == '@')
				return (1);
			i++;
		}
	}
	return (0);
}

void	process_existing_env(t_envi **env, char *arr[2])
{
	t_envi	*tmp;

	tmp = (*env);
	while (tmp)
	{
		if (!strcmp(tmp->name, arr[0]))
			tmp->vale = ft_strdup_1(arr[1]);
		tmp = tmp->next;
	}
}

int	process_single_env(char *ptr_i, t_envi **env)
{
	char	*arr[2];
	t_envi	*new;
	int		status;
	char	*tmp;

	status = 0;
	tmp = ft_strdup(ptr_i);
	arr[0] = strtok(ptr_i, "=");
	arr[1] = strtok(NULL, "=");
	if (!arr[1] && tmp[strlen(tmp) - 1] != '=')
		return (status);
	else if (!arr[1] && tmp[strlen(tmp) - 1] == '=')
		arr[1] = ft_strdup("");
	if (ft_utils(arr[0]))
	{
		new = search_env(*env, arr[0]);
		if (new)
			process_existing_env(env, arr);
		else
			add_env_variable(env, arr[0], arr[1]);
	}
	return (status);
}

int	add_one(char **ptr, t_envi **env)
{
	int	i;
	int	status;
	int	result;

	i = 1;
	status = 0;
	while (ptr[i])
	{
		if (check_each_element(ptr[i]))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(ptr[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			g_var->exit_status = 1;
		}
		else
		{
			result = process_single_env(ptr[i], env);
			if (result)
				status = result;
		}
		i++;
	}
	g_var->envp = *env;
	return (status);
}

int	ft_export(char **ptr)
{
	t_envi	*newenv;
	int		status;

	status = 0;
	if (!ptr[1])
	{
		newenv = sort_env(g_var->envp);
		if (!newenv)
		{
			perror("Failed to sort environment");
			return (1);
		}
		while (newenv)
		{
			printf("declare -x %s", newenv->name);
			if (newenv->vale)
				printf("=\"%s\"", newenv->vale);
			printf("\n");
			newenv = newenv->next;
		}
	}
	else
		status = add_one(ptr, &g_var->envp);
	return (status);
}
