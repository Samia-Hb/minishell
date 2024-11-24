/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main->c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szeroual <szeroual@student->42->fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-11-21 17:50:29 by szeroual          #+#    #+#             */
/*   Updated: 2024-11-21 17:50:29 by szeroual         ###   ########->fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

struct global				*g_var;

void	initiale_global(t_envi *env)
{
	g_var = malloc(sizeof(struct global));
	g_var->exit_status = 0;
	g_var->head = NULL;
	g_var->pre_pipe_infd = -1;
	g_var->envp = env;
	g_var->last_child_id = 0;
	g_var->out_fd = STDOUT_FILENO;
	g_var->in_fd = STDIN_FILENO;
	g_var->red_error = 0;
	g_var->size = 0;
	g_var->pipe_nb = 0;
}
// void	init_g_var(t_cmd **token)
// {
// 	t_cmd	*current;

// 	g_var->red_error = 0;
// 	g_var->pre_pipe_infd = -1;
// 	g_var->last_child_id = 0;
// 	// g_var->interactive = 1;
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

void	print_cmd(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd)
	{
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

t_envi	*init_env(char **en)
{
	t_envi	*env_list;
	char	*env_entry;
	int		i;

	env_list = NULL;
	i = 0;

	while (en[i])
	{
		env_entry = ft_strdup(en[i]);
		if (!env_entry)
			error_strdup();
		process_env_entry(env_entry, &env_list);
		i++;
	}
	return (env_list);
}

int	main(int argc, char **argv, char **envp)
{
	t_mini	*box;

	(void)argc;
	(void)argv;
	envp = envp;
	box = malloc(sizeof(t_mini));
	if (!box)
		error_malloc();
	initialisation(box, envp);
	shell_loop(box);
	return (0);
}
