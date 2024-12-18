/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 10:19:04 by szeroual          #+#    #+#             */
/*   Updated: 2024/12/18 11:03:40 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

char	*set_node_vale(const char *vale)
{
	if (vale)
		return (ft_strdup_1(vale));
	return (ft_strdup_1(""));
}

void	free_node_contents(t_envi *node)
{
	if (!node)
		return ;
	free(node->name);
	free(node->vale);
	free(node);
}

void	update_existing_node(t_envi *current, const char *vale, int had_equals)
{
	if (had_equals == 1)
	{
		free(current->vale);
		current->vale = set_node_vale(vale);
		if (!current->vale)
			perror("malloc");
	}
}

int	count_valid_env_entries(t_envi *env)
{
	int		count;
	t_envi	*current;

	count = 0;
	current = env;
	while (current)
	{
		if (current->vale != NULL)
			count++;
		current = current->next;
	}
	return (count);
}

char	*create_env_string(t_envi *env_node)
{
	char	*env_str;
	size_t	len;

	len = ft_strlen(env_node->name) + 1;
	if (env_node->vale)
		len += ft_strlen(env_node->vale);
	env_str = (char *)malloc(len + 1);
	if (!env_str)
		return (NULL);
	ft_strlcpy(env_str, env_node->name, len + 1);
	ft_strlcat(env_str, "=", len + 1);
	if (env_node->vale)
		ft_strlcat(env_str, env_node->vale, len + 1);
	return (env_str);
}
