#include "../../../minishell.h"

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
	{
		return (ft_strdup(vale));
	}
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

void	sync_env_array(t_envi *env)
{
	char	**new_env;

	new_env = separate_env(env);
	if (!new_env)
		return ;
	// free_env_array(g_var->en);
}
