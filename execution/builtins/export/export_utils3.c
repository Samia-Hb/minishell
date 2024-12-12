#include "../../../minishell.h"

// t_envi	*create_and_init_node(const char *var, const char *vale,
// 		int had_equals)
// {
// 	t_envi	*new_node;

// 	new_node = (t_envi *)malloc(sizeof(t_envi));
// 	if (!new_node)
// 	{
// 		perror("malloc");
// 		return (NULL);
// 	}
// 	new_node->name = ft_strdup(var);
// 	if (had_equals == 1)
// 		new_node->vale = set_node_vale(vale);
// 	else
// 		new_node->vale = NULL;
// 	if (!new_node->name || (had_equals && !new_node->vale))
// 	{
// 		free_node_contents(new_node);
// 		return (NULL);
// 	}
// 	new_node->next = NULL;
// 	return (new_node);
// }

// 



void	update_or_add_env(t_envi **env, const char *var, const char *vale,
		int had_equals)
{
	t_envi	*current;
	t_envi	*prev;
	t_envi	*new_node;

	update_or_add_env_part1(env, var, &prev, &current);
	if (current)
	{
		update_existing_node(current, vale, had_equals);
		return ;
	}
	new_node = create_and_init_node(var, vale, had_equals);
	if (!new_node)
		return ;
	if (prev)
		prev->next = new_node;
	else
		*env = new_node;
}

// void	process_cmd(t_envi **env, char **cmd, int i)
// {
// 	char	*eq_pos;
// 	char	*var;
// 	char	*vale;
// 	int		had_equals;

// 	eq_pos = ft_strchr(cmd[i], '=');
// 	var = cmd[i];
// 	vale = NULL;
// 	had_equals = 0;
// 	if (eq_pos)
// 	{
// 		*eq_pos = '\0';
// 		vale = eq_pos + 1;
// 		had_equals = 1;
// 	}
// 	if (!is_valid_identifier(var))
// 	{
// 		handle_invalid_identifier(cmd[i]);
// 		if (eq_pos)
// 			*eq_pos = '=';
// 		return ;
// 	}
// 	update_or_add_env(env, var, vale, had_equals);
// 	if (eq_pos)
// 		*eq_pos = '=';
// }

void	handle_invalid_identifier(const char *cmd)
{
	char	*tmp;

	ft_putstr_fd("minishell: export: `", 2);
	tmp = (char *)cmd;
	ft_putstr_fd(tmp, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	g_var->exit_status = 1;
}
