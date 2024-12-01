/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 00:12:31 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/01 11:51:07 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

struct s_global	*g_var;

// void	init_g_var(t_cmd **token)
// {
// 	t_cmd	*current;

// 	g_var->red_error = 0;
// 	g_var->flag = 0;
// 	g_var->pre_pipe_infd = -1;
// 	g_var->last_child_id = 0;
// 	g_var->size = 0;
// 	g_var->out_fd = 1;
// 	g_var->num = 0;
// 	current = *token;
// 	while (current)
// 	{
// 		current->in_fd = 0;
// 		current->out_fd = 0;
// 		current = current->next;
// 	}
// }

void	error_malloc(void)
{
	perror("malloc");
	exit(EXIT_FAILURE);
}

t_envi	*init_env(char **en)
{
	t_envi	*env_list;
	int		i;
	char	*env_entry;

	env_list = NULL;
	i = 0;
	while (en[i])
	{
		env_entry = strdup(en[i]);
		if (!env_entry)
			error_strdup();
		process_env_entry(env_entry, &env_list);
		i++;
	}
	return (env_list);
}
void	print_cmd(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd)
	{
		printf("=======Arguments=======\n");
		if (cmd->arguments)
		{
			i = 0;
			while (cmd->arguments[i])
			{
				printf("arg[%d] == %s\n", i, cmd->arguments[i]);
				i++;
			}
		}
		if (cmd->file)
		{
			while (cmd->file)
			{
				printf("filename == %s type == %d\n", cmd->file->filename,
					cmd->file->type);
				cmd->file = cmd->file->next;
			}
		}
		cmd = cmd->next;
	}
}

int	main(int argc, char **argv, char **en)
{
	t_mini	*box;

	(void)argc;
	(void)argv;
	box = malloc(sizeof(t_mini));
	if (!box)
		error_malloc();
	initialisation(box, en);
	shell_loop(box);
	return (0);
}
