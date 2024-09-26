#include "minishell.h"

int	main()
{
    char	    *input;
    Token	    **tokens;
    t_queue     *queue;
    t_ast       *ast;
    t_parser    *parsed;

	(void)ast;
	tokens = NULL;
	while (1)
	{
		handle_signal();
		input = readline("Minishell$ ");
		if (!input)
			break ;
        if (!strlen(input))
            main();
        add_history(input);
		tokens = tokenize(input);
		while (*tokens)
		{
			printf("tokens = %s\n", (*tokens)->value);
			(*tokens) = (*tokens)->next;
		}
		exit(1);
        expand (*tokens);
		Token *token = *tokens;
		printf("======== expanded result =======\n");
		int i;
		while (token)
		{
			i = 0;
			printf("token = %s\n", token->value);
			if (token->expanded_value)
			{
				while (token->expanded_value[i])
				{
					printf("expanded_arg[%d] = %s\n",i, token->expanded_value[i]);
					i++;
				}
			}
			token = token->next;
		}
		parsed = analyse_tokens(tokens);
		printf("======== Analysed result =======\n");
		while (parsed)
		{
			i = 0;
			printf ("token = %s\n", parsed->token->value);
			if(parsed->arguments)
			{
				while (parsed->arguments[i])
				{
					printf("       arg[%d] = %s\n",i, parsed->arguments[i]);
					i++;
				}
			}
			parsed = parsed->next;
		}
		main();
		queue = generate_postfix(parsed);
		ast = generate_ast_from_postfix(queue);
		print_ast(ast,5);
	}
	return (0);
}