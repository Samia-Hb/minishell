
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

char	*set_node_vale(const char *vale)
{
	if (vale)
		return (ft_strdup(vale));
	return (ft_strdup(""));
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
static int	count_valid_env_entries(t_envi *env)
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
// static void	init_copy_env(t_envi **new_env, t_envi **last_node)
// {
// 	*new_env = NULL;
// 	*last_node = NULL;
// }
static char	*create_env_string(t_envi *env_node)
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
static int	populate_env_array(char **env_array, t_envi *env, int count)
{
	int		i;
	t_envi	*current;

	i = 0;
	current = env;
	while (current && i < count)
	{
		if (current->vale != NULL)
		{
			env_array[i] = create_env_string(current);
			if (!env_array[i])
				return (0);
			i++;
		}
		current = current->next;
	}
	env_array[i] = NULL;
	return (1);
}

char	**env_to_array(t_envi *env)
{
	int		count;
	char	**env_array;
	// int		i;

	count = count_valid_env_entries(env);
	env_array = (char **)malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	if (!populate_env_array(env_array, env, count))
	{
		// i = 0;
		// while (env_array[i])
		// 	free(env_array[i++]);
		// free(env_array);
		return (NULL);
	}
	return (env_array);
}
void	sync_env_array(t_envi *env)
{
	char	**new_env;

	new_env = env_to_array(env);
	if (!new_env)
		return ;
	// free_env_array(g_var->en);
	g_var->en= new_env;
}
static void	init_copy_env(t_envi **new_env, t_envi **last_node)
{
	*new_env = NULL;
	*last_node = NULL;
}

static t_envi	*process_env_node(t_envi *current, t_envi **last_node)
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

t_envi	*bubble_sort_env(t_envi *env)
{
	int		swapped;
	t_envi	*ptr1;
	t_envi	*lptr;

	if (!env)
		return (NULL);
	lptr = NULL;
	while (1)
	{
		swapped = 0;
		ptr1 = env;
		while (ptr1->next != lptr)
		{
			if (ft_strcmp(ptr1->name, ptr1->next->name) > 0)
			{
				swap_nodes(ptr1, ptr1->next);
				swapped = 1;
			}
			ptr1 = ptr1->next;
		}
		lptr = ptr1;
		if (!swapped)
			break ;
	}
	return (env);
}

void	print_export(t_envi *env)
{
	t_envi	*current;

	current = bubble_sort_env(env);
	while (current)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(current->name, 1);
		if (current->vale != NULL)
		{
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(current->vale, 1);
			ft_putstr_fd("\"", 1);
		}
		ft_putstr_fd("\n", 1);
		current = current->next;
	}
}

void	handle_no_cmd(t_envi **env)
{
	t_envi	*env_copy;

	env_copy = copy_env(*env);
	if (!env_copy)
		return ;
	print_export(env_copy);
	// free_env(env_copy);
}

void	ft_export(t_envi **env, char **cmd)
{
	int	i;

	g_var->exit_status = 0;
	if (cmd[1] == NULL)
	{
		handle_no_cmd(env);
		return ;
	}
	i = 1;
	while (cmd[i])
	{
		process_cmd(env, cmd, i);
		i++;
	}
	sync_env_array(*env);
}
