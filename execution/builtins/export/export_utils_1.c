#include "../../../minishell.h"

t_envi	*create_and_init_node(const char *var, const char *vale,
		int had_equals)
{
	t_envi	*new_node;

	new_node = (t_envi *)malloc(sizeof(t_envi));
	if (!new_node)
	{
		perror("malloc");
		return (NULL);
	}
	new_node->name = ft_strdup(var);
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

void	update_or_add_env_part1(t_envi **env, const char *var, t_envi **prev,
		t_envi **current)
{
	*current = *env;
	*prev = NULL;
	while (*current)
	{
		if (ft_strcmp((*current)->name, var) == 0)
			return ;
		*prev = *current;
		*current = (*current)->next;
	}
}

// void	update_or_add_env(t_envi **env, const char *var, const char *vale,
// 		int had_equals)
// {
// 	t_envi	*current;
// 	t_envi	*prev;
// 	t_envi	*new_node;

// 	update_or_add_env_part1(env, var, &prev, &current);
// 	if (current)
// 	{
// 		update_existing_node(current, vale, had_equals);
// 		return ;
// 	}
// 	new_node = create_and_init_node(var, vale, had_equals);
// 	if (!new_node)
// 		return ;
// 	if (prev)
// 		prev->next = new_node;
// 	else
// 		*env = new_node;
// }

void	init_copy_env(t_envi **new_env, t_envi **last_node)
{
	*new_env = NULL;
	*last_node = NULL;
}
t_envi	*process_env_node(t_envi *current, t_envi **last_node)
{
	t_envi	*new_node;
	int		has_vale;

	has_vale = 0;
	if (current->vale != NULL)
		has_vale = 1;
	new_node = create_new_node(current->name, current->vale, has_vale);
	if (!new_node)
		return (NULL);
	if (*last_node)
		(*last_node)->next = new_node;
	*last_node = new_node;
	return (new_node);
}

t_envi	*copy_env(t_envi *env)
{
	t_envi	*new_env;
	t_envi	*current;
	t_envi	*last_node;
	t_envi	*new_node;

	init_copy_env(&new_env, &last_node);
	current = env;
	while (current)
	{
		new_node = process_env_node(current, &last_node);
		if (!new_node)
		{
			// free_env(new_env);
			return (NULL);
		}
		if (!new_env)
			new_env = new_node;
		current = current->next;
	}
	return (new_env);
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

