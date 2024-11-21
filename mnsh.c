/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mnsh.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 15:53:32 by shebaz            #+#    #+#             */
/*   Updated: 2024/11/21 15:53:46 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_globalvar	*g_var = NULL;

t_shell	*init_shell(void)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	shell->exit_status = 0;
	shell->args = NULL;
	return (shell);
}

t_envi	*init_env(char **envp)
{
	t_envi	*env_list;
	t_envi	*new_node;
	char	*name;
	char	*value;
	int		i;
	char	*env_entry;

	env_list = NULL;
	i = 0;
	while (envp[i])
	{
		env_entry = ft_strdup(envp[i]);
		if (!env_entry)
		{
			perror("strdup");
			exit(EXIT_FAILURE);
		}
		name = strtok(env_entry, "=");
		value = strtok(NULL, "=");
		if (!name || !value)
		{
			perror("error");
			free(env_entry);
			i++;
			continue ;
		}
		new_node = malloc(sizeof(t_envi));
		if (!new_node)
		{
			perror("malloc");
			clean_gc();
			exit(EXIT_FAILURE);
		}
		new_node->name = ft_strdup(name);
		new_node->vale = ft_strdup(value);
		if (!new_node->name || !new_node->vale)
		{
			perror("strdup");
			clean_gc();
			exit(EXIT_FAILURE);
		}
		new_node->next = env_list;
		new_node->prv = NULL;
		if (env_list)
			env_list->prv = new_node;
		env_list = new_node;
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

void	initiale_global(t_envi *env)
{
	g_var = malloc(sizeof(t_globalvar));
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

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_token	**tokens;
	t_cmd	*cmd;
	t_mini	*box;

	(void)argc;
	(void)argv;
	tokens = NULL;
	box = malloc(sizeof(t_mini));
	if (!box)
	{
		write(2, "Error\n", 6);
		exit(EXIT_FAILURE);
	}
	box->env = init_env(envp);
	box->shell = init_shell();
	box->ptr = NULL;
	box->arr = NULL;
	box->last_exit_status = 0;
	initiale_global(box->env);
	while (1)
	{
		handle_signal();
		input = readline("minishell > ");
		if (!input)
			break ;
		if (!ft_strlen(input))
			continue ;
		add_history(input);
		tokens = tokenize(input);
		if (check_syntax_errors(*tokens))
			continue ;
		if (!expand(*tokens))
			continue ;
		cmd = analyse_tokens(tokens);
		// print_cmd(cmd);
		// exit(1);
		execute_arguments(cmd, box);
	}
	return (0);
}
