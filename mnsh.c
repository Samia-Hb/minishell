/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mnsh.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 15:53:32 by shebaz            #+#    #+#             */
/*   Updated: 2024/11/02 16:24:54 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
				printf("filename == %s\n", cmd->file->filename);
				cmd->file = cmd->file->next;
			}
		}
		cmd = cmd->next;
	}
}

int	main()
{
	char	*input;
	Token	**tokens;
	t_cmd	*cmd;

	tokens = NULL;
	while (1)
	{
		handle_signal();
		input = readline("minishell > ");
		if (!input)
			break ;
		add_history(input);
		tokens = tokenize(input);
		if (check_syntax_errors(*tokens))
			main();
		if (!expand(*tokens))
			return (0);
		cmd = analyse_tokens(tokens);
		print_cmd(cmd);
	}
	return (0);
}
