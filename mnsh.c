/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mnsh.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 15:53:32 by shebaz            #+#    #+#             */
/*   Updated: 2024/11/19 10:04:23 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_globalvar *g_var = NULL;

void	initiale_global(t_envi *env)
{
	g_var = malloc(sizeof(t_globalvar));
	g_var->exit_status = 0;
	g_var->head = NULL;
	g_var->pre_pipe_infd = -1;
	g_var->envp = env;
	g_var->last_child_id = 0;
	g_var->out_fd = -1;
	g_var->red_error = 0;
	g_var->size = 0;
	g_var->pipe_nb = 0;
}

t_shell *init_shell()
{
    t_shell *shell = malloc(sizeof(t_shell));
    if (!shell)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    shell->exit_status = 0;
    shell->args = NULL;
    return shell;
}

void error_strdup()
{
    perror("strdup");
    exit(EXIT_FAILURE);
}

t_envi *create__node(char *name, char *value)
{
    t_envi *new_node = malloc(sizeof(t_envi));
    if (!new_node)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    new_node->name = strdup(name);
    new_node->vale = strdup(value);
    if (!new_node->name || !new_node->vale)
    {
        perror("strdup");
        free(new_node->name);
        free(new_node->vale);
        free(new_node);
        exit(EXIT_FAILURE);
    }
    new_node->next = NULL;
    new_node->prv = NULL;
    return new_node;
}

void add_env_node(t_envi **env_list, t_envi *new_node)
{
    if (*env_list)
    {
        (*env_list)->prv = new_node;
    }
    new_node->next = *env_list;
    *env_list = new_node;
}

void process_env_entry(char *env_entry, t_envi **env_list)
{
    char *name = strtok(env_entry, "=");
    char *value = strtok(NULL, "=");
    if (!name || !value)
    {
        perror("error");
        free(env_entry);
        return;
    }
    t_envi *new_node = create__node(name, value);
    add_env_node(env_list, new_node);
    free(env_entry);
}

t_envi *init_env(char **envp)
{
    t_envi *env_list = NULL;
    int i = 0;

    while (envp[i])
    {
        char *env_entry = strdup(envp[i]);
        if (!env_entry)
        {
            error_strdup();
        }
        process_env_entry(env_entry, &env_list);
        i++;
    }
    return env_list;
}

void error_malloc()
{
    perror("malloc");
    exit(EXIT_FAILURE);
}

void init_box(t_mini *box, char **envp)
{
    box->env = init_env(envp);
    box->shell = init_shell();
    box->ptr = NULL;
    box->arr = NULL;
    box->last_exit_status = 0;
}

void initialisation(t_mini *box, char **envp)
{
    initiale_global(init_env(envp));
    init_box(box, envp);
}

void handle_input(char *input, t_mini *box)
{
    t_token **tokens;
    t_cmd *cmd;

    if (!ft_strlen(input))
        return;
    add_history(input);
    tokens = tokenize(input);
    if (check_syntax_errors(*tokens))
        return;
    if (!expand(*tokens))
        return;
    cmd = analyse_tokens(tokens);
    execute_arguments(cmd, box);
}

void shell_loop(t_mini *box)
{
    char *input;

    while (1)
    {
        handle_signal();
        input = readline("minishell > ");
        if (!input)
            break;
        handle_input(input, box);
        free(input);
    }
}

int main(int argc, char **argv, char **envp)
{
    t_mini *box;

    (void)argc;
    (void)argv;

    box = malloc(sizeof(t_mini));
    if (!box)
        error_malloc();
    initialisation(box, envp);
    shell_loop(box);
    return 0;
}

