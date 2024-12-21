/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_util1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 22:29:05 by szeroual          #+#    #+#             */
/*   Updated: 2024/12/21 01:42:08 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strdup_1(const char *s1)
{
	char	*s2;
	size_t	i;

	i = 0;
	s2 = malloc((ft_strlen(s1) + 1) * sizeof(char));
	if (!s2)
		return (NULL);
	while (s1[i] != '\0')
	{
		s2[i] = s1[i];
		i++;
	}
	s2[i] = '\0';
	return (s2);
}

void	error_strdup(void)
{
	perror("strdup");
	exit(EXIT_FAILURE);
}

void	initiale_global(t_envi *env)
{
	g_var = ft_malloc(1, sizeof(struct s_global));
	if (!g_var)
	{
		perror("calloc failed");
		exit(EXIT_FAILURE);
	}
	g_var->envp = env;
	g_var->cpy_in_fd = -1;
	g_var->cpy_out_fd = -1;
	g_var->exit_status = 0;
	g_var->flag = 0;
	g_var->pid_size = 0;
	g_var->i = 0;
	g_var->status = 0;
	g_var->stop = 0;
}

void	handle_input(char *input, t_envi *envp)
{
	t_token	**tokens;
	t_cmd	*cmd;

	if (!ft_strlen(input))
		return ;
	add_history(input);
	tokens = tokenize(input);
	if (check_syntax_errors(*tokens))
		return ;
	if (!expand(*tokens))
		return ;
	// while (*tokens)
	// {
	// 	printf("value_here = %s\n", (*tokens)->value);
	// 	if ((*tokens)->expanded_value)
	// 	{
	// 		int i = 0;
	// 		while ((*tokens)->expanded_value[i])
	// 		{
	// 			printf("==>arg[%d] = %s\n",i ,(*tokens)->expanded_value[i]);
	// 			i++;
	// 		}
	// 	}
	// 	(*tokens) = (*tokens)->next;
	// }
	// exit(1);
	cmd = analyse_tokens(tokens);
	if (g_var->flag == 7)
		return ;
	g_var->status = g_var->exit_status;
	g_var->exit_status = 0;
	execute_arguments(cmd, envp);
}

void	shell_loop(t_envi *envp)
{
	char	*input;
	int		exit_status;

	while (1)
	{
		handle_signal();
		g_var->flag = 0;
		input = readline("minishell > ");
		if (!input)
		{
			exit_status = g_var->exit_status;
			printf("exit\n");
			rl_clear_history();
			clean_gc();
			ft_free_envp(envp);
			close(STDOUT_FILENO);
			close(STDIN_FILENO);
			close(STDERR_FILENO);
			exit(exit_status);
		}
		handle_input(input, envp);
		g_var->stop = 0;
	}
}
