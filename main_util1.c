/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_util1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 22:29:05 by szeroual          #+#    #+#             */
/*   Updated: 2024/12/02 21:11:56 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error_strdup(void)
{
	perror("strdup");
	exit(EXIT_FAILURE);
}

void	initiale_global(t_envi *env)
{
	g_var = malloc(sizeof(struct s_global));
	if (!g_var)
	{
		perror("malloc failed");
		exit(EXIT_FAILURE);
	}
	g_var->envp = env;
	g_var->exit_status = 0;
	g_var->red_error = 0;
	g_var->flag = 0;
}

void	handle_input(char *input, t_mini *box)
{
	t_token	**tokens;
	t_cmd	*cmd;

	if (!ft_strlen(input))
		return ;
	add_history(input);
	tokens = tokenize(input);
	if (check_syntax_errors(*tokens))
	{
		g_var->exit_status = 2;
		return ;
	}
	if (!expand(*tokens))
		return ;
	cmd = analyse_tokens(tokens);
	if (g_var->flag == 7)
		return ;
	execute_arguments(cmd, box);
}

void	shell_loop(t_mini *box)
{
	char	*input;

	while (1)
	{
		handle_signal();
		g_var->flag = 0;
		input = readline("minishell > ");
		if (!input)
			break ;
		handle_input(input, box);
		free(input);
	}
}
