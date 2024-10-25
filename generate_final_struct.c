#include "minishell.h"


void push_back(t_cmd **lst, t_cmd *node)
{
    t_cmd *tmp;

    if (!*lst)
        *lst = node;
    else
    {
        tmp = *lst;
        while(tmp->next)
            tmp = tmp->next;
        tmp->next = node;
    }
}

int nbr_argument(Token *tokens)
{
    int nbr;
    int	i;

    nbr = 0;
    while (tokens && !is_operator(tokens))
    {
    	i = 0;
        while (tokens->expanded_value[i])
        {
            i++;
            nbr++;
        }
        tokens = tokens->next;
    }
    return (nbr);
}

void red_process(Token **tokens, t_cmd **node)
{
    if ((*tokens)->type == TOKEN_REDIR_IN)
        (*node)->file->type = RE_IN;
    else if ((*tokens)->type == TOKEN_REDIR_OUT)
        (*node)->file->type = RE_OUT;
    else if ((*tokens)->type == TOKEN_REDIR_APPEND)
        (*node)->file->type = RE_APPEND;
    (*node)->file = (*tokens)->next->value;
    (*tokens) = (*tokens)->next;
}
void create_node_arguments(t_cmd **node, Token **tokens)
{
    int i;
	int j;
    int num_args;

    i = 0;
    num_args = nbr_argument(*tokens);
    (*node)->arguments = malloc((num_args + 1) * sizeof(Token *));
    if (!(*node)->arguments)
    {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    (*node)->arguments[0] = (*tokens)->expanded_value[0];
    (*tokens) = (*tokens)->next;
    i++;
    while (*tokens && (*tokens)->type != TOKEN_PIPE &&
           (*tokens)->type != TOKEN_COMMAND && (*tokens)->type != TOKEN_BUILT_IN)
           {
				j = 0;
				while ((*tokens)->expanded_value[j])
				{
					(*node)->arguments[i] = (*tokens)->expanded_value[j];
					i++;
					j++;
				}
                if ((*tokens)->type == TOKEN_REDIR_APPEND || (*tokens)->type == TOKEN_REDIR_IN || (*tokens)->type == TOKEN_REDIR_OUT)
                {
                    rede_process(tokens, node);
				    (*tokens) = (*tokens)->next;
                }                
				(*tokens) = (*tokens)->next;
    		}
    (*node)->arguments[i] = NULL;
}
void free_tokens(Token *tokens)
{

    Token *tmp;
    while (tokens)
    {
        tmp = tokens;
        tokens = tokens->next;        
        if (tmp->value)
            free(tmp->value);
        if (tmp->expanded_value)
        {
            for (int i = 0; tmp->expanded_value[i]; i++)
            {
                free(tmp->expanded_value[i]);
            }
            free(tmp->expanded_value);
        }
        free(tmp);
    }
}

t_cmd *analyse_tokens(Token **tokens)
{
    t_cmd *node;

    while (*tokens)
    {
        node = malloc(sizeof(t_cmd));
        if (!node)
        {
            printf("allocation failed\n");
            exit(1);
        }
        if ((*tokens)->type == TOKEN_COMMAND || (*tokens)->type == TOKEN_BUILT_IN)
            create_node_arguments(&node, tokens);
		else
            (*tokens) = (*tokens)->next;
        push_back(&node, node);
    }
    return node;
}