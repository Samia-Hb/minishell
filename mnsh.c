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
void error_strdup()
{
      perror("strdup");
      exit(EXIT_FAILURE);
}

t_envi *init_env(char **envp)
{
    t_envi *env_list;
    char *name;
    char *value;
    int i;
    
    i = 0;
    env_list = NULL;
    while (envp[i])
    {
        char *env_entry = strdup(envp[i]);
        if (!env_entry)
          error_strdup();
        name = strtok(env_entry, "=");
        value = strtok(NULL, "=");
        if (!name || !value)
        {
            perror("error");
            free(env_entry);
            i++;
            continue;
        }
        t_envi *new_node = create__node(name, value);
        add_env_node(&env_list, new_node);
        free(env_entry);
        i++;
    }
    return env_list;
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

int main(int argc, char **argv, char **envp)
{
    char *input;
    t_token **tokens;
    t_cmd *cmd;
    t_mini *box;
    (void)argc;
    (void)argv;

    tokens = NULL;
    box = malloc(sizeof(t_mini));
    if (!box)
        error_malloc();
    initialisation(box, envp);
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
            continue;
        cmd = analyse_tokens(tokens);
        execute_arguments(cmd, box);
        free(input);
    }
    return 0;
}
