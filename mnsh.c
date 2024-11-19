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

<<<<<<< HEAD
// void ft_clean(Token **tokens, t_parser *parsed, t_queue *queue)
// {
// 	int i;

// 	i = 0;
// 	if(tokens)
// 	{
// 		while(*tokens)
// 		{
// 			free(tokens[i]->value);
// 			i++;
// 			(*tokens) = (*tokens)->next;
// 		}
// 		free(tokens);
// 	}
// 	if (parsed)
// 		free(parsed);
// 	if (queue)
// 		free(queue);
// }
int	main()
{
    char	    *input;
    Token	    **tokens;
	int			errno;
    t_cmd    	*cmd;
=======
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
>>>>>>> main

t_envi *init_env(char **envp)
{
    t_envi *env_list = NULL;
    t_envi *new_node;
    char *name;
    char *value;
    int i = 0;

    while (envp[i])
    {
        char *env_entry = strdup(envp[i]);
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
            continue;
        }
        new_node = malloc(sizeof(t_envi));
        if (!new_node)
        {
            perror("malloc");
            free(env_entry);
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
            free(env_entry);
            exit(EXIT_FAILURE);
        }
        new_node->next = env_list;
        new_node->prv = NULL;
        if (env_list)
            env_list->prv = new_node;
        env_list = new_node;
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
    {
        perror("malloc");
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
<<<<<<< HEAD
		tokens = tokenize(input);
		errno = check_syntax_errors(*tokens);
        if (errno)
            main();
        expand(*tokens);
		// cmd = generate_final_struct(toknes);
		cmd = analyse_tokens(tokens);
		(void)cmd;
		// queue = generate_postfix(parsed);
		// ast = generate_ast_from_postfix(queue);
		// rl_clear_history();
		// exit(1);
	}
	return (0);
}



		// Token *token = *tokens;
		// int i ;
		// while (token)
		// {
		// 	i = 0;
		// 	printf("token = %s\n", token->value);
		// 	if (token->expanded_value)
		// 	{
		// 		while (token->expanded_value[i])
		// 		{
		// 			printf("expanded_arg[%d] = %s\n",i, token->expanded_value[i]);
		// 			i++;
		// 		}
		// 	}
		// 	token = token->next;
		// }
		// exit(1);





		// printf("======== Analysed result =======\n");
		// while (parsed)
		// {
		// 	i = 0;
		// 	printf ("token = %s\n", parsed->token->value);
		// 	if(parsed->arguments)
		// 	{
		// 		while (parsed->arguments[i])
		// 		{
		// 			printf("       arg[%d] = %s\n",i, parsed->arguments[i]);
		// 			i++;
		// 		}
		// 	}
		// 	parsed = parsed->next;
		// }
		// main();

		/// ///////////////////////////////////
		// int i;
=======
        tokens = tokenize(input);
        if (check_syntax_errors(*tokens))
            continue ;
        if (!expand(*tokens))
            continue;
        cmd = analyse_tokens(tokens);
        // print_cmd(cmd);
        // exit(1);
        execute_arguments(cmd, box);
        free(input);
        // clean_gc();
    }
    return 0;
}
>>>>>>> main
