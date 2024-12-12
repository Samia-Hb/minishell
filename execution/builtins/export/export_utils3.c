#include "../../../minishell.h"


void	update_or_add_env(t_envi **env, const char *var, const char *vale, int had_equals)
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
    new_node = create_new_node(var, vale, had_equals);
    if (!new_node || !new_node->vale)
        return ;
    if (prev)
        prev->next = new_node;
    else
        *env = new_node;
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
