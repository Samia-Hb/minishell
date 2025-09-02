/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.utils1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 10:18:53 by szeroual          #+#    #+#             */
/*   Updated: 2024/12/23 23:11:37 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

void	swap_nodes(t_envi *a, t_envi *b)
{
	char	*temp_name;
	char	*temp_vale;

	temp_name = a->name;
	temp_vale = a->vale;
	a->name = b->name;
	a->vale = b->vale;
	b->name = temp_name;
	b->vale = temp_vale;
}

int	is_valid_identifier(const char *str)
{
	if (!str || !*str || !(ft_isalpha(*str) || *str == '_'))
		return (0);
	while (*++str)
		if (!(ft_isalnum(*str) || *str == '_'))
			return (0);
	return (1);
}

void	process_cmd(t_envi **env, char **cmd, int i)
{
	char	*eq_pos;
	char	*var;
	char	*vale;
	int		had_equals;

	eq_pos = ft_strchr(cmd[i], '=');
	var = cmd[i];
	vale = NULL;
	had_equals = 0;
	if (eq_pos)
	{
		*eq_pos = '\0';
		vale = eq_pos + 1;
		had_equals = 1;
	}
	if (!is_valid_identifier(var))
	{
		handle_invalid_identifier(cmd[i]);
		if (eq_pos)
			*eq_pos = '=';
		return ;
	}
	update_or_add_env(env, var, vale, had_equals);
	if (eq_pos)
		*eq_pos = '=';
}

void	handle_invalid_identifier(const char *cmd)
{
	char	*tmp;

	ft_putstr_fd("minishell: export: `", 2);
	tmp = (char *)cmd;
	ft_putstr_fd(tmp, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	g_var->exit_status = 1;
}

t_envi	*create_new_node(const char *var, const char *vale, int had_equals)
{
	t_envi	*new_node;

	new_node = (t_envi *)malloc(sizeof(t_envi));
	if (!new_node)
	{
		perror("malloc");
		return (NULL);
	}
	new_node->name = ft_strdup_1(var);
	if (had_equals == 1)
		new_node->vale = set_node_vale(vale);
	else
		new_node->vale = NULL;
	if (!new_node->name || (had_equals && !new_node->vale))
	{
		free_node_contents(new_node);
		return (NULL);
	}
	new_node->next = NULL;
	return (new_node);
}
