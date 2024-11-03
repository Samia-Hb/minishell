/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 15:39:23 by shebaz            #+#    #+#             */
/*   Updated: 2024/11/02 16:51:37 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "externel_folder/gnl/get_next_line.h"
# include "externel_folder/libftt/libft.h"
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define SIGINT 2 
# define SIGQUIT 3
# define SIGTERM 15

typedef enum
{
	TOKEN_TILDLE,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_DOUBLE_QUOTED,
	TOKEN_SINGLE_QUOTED,
	TOKEN_OPEN_PARENTH,
	TOKEN_CLOSE_PARENTH,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_REDIR_HERE_DOC,
	TOKEN_COMMAND,
	DELIMITER,
	TOKEN_OPTION,
	TOKEN_BUILT_IN,
	TOKEN_ARGUMENT,
	TOKEN_UNKNOWN
}					TokenType;

typedef struct token
{
	TokenType		type;
	char			*value;
	char			**expanded_value;
	struct token	*next;
	struct token	*previous;
}					Token;

typedef struct s_file
{
	char			*filename;
	int				type;
	struct s_file	*next;
}					t_file;

typedef struct s_cmd
{
	char			**arguments;
	t_file			*file;
	struct s_cmd	*prev;
	struct s_cmd	*next;
}					t_cmd;

typedef enum
{
	PIPE,
	RE_OUT,
	RE_IN,
	RE_HEREDOC,
	RE_APPEND,
	UNKOWN
}					t_type;

Token				**tokenize(char *input);
char				*handle_quote(char *str);
int					is_quoted(char *input);
char				*expand_non_operator(char *token);
int					built_in_checker(const char *str);
void				add_token(Token **tokens,
						TokenType type, char *value, int *k);
char				*get_executable(char *command);
char				*process_delimiter(char *tmp);
void				handle_heredoc(Token **tokens, char *input, int *i);
void				heredoc_process(t_cmd **node, t_file **head ,Token **tokens);
char				*tidle_expansion(int *i);
char				*dollar_expand(char *input, int *i);
char				**result_traitement(char *input);
char				*get_string(char *input, int *i);
int					get_size(char **arr);
int					get_size_arr(char *input);
char 				*parse_line(char *input);
char				**handle_that_shit(char *input);
char				**unquoted_result(char **input);
char				*get_word_to_expand(char *str, int *j);
void				add_quote(char *input, char **expanded_value, int *j);
int					is_operator(Token *node);
int					is_operand(Token *node);
int					handle_consecutive_operator(Token *tokens);
int					handle_paren(Token *token);
int					handle_quotes(Token *tokens);
int					check_token(char *str, char c);
Token				*get_last_token(Token *token);
int					handle_operators_bg_en(Token *tokens);
int					check_syntax_errors(Token *tokens);
char				quote_type(const char *str);
char				*char_to_string(char c, char c2);
int					get_token_type(const char *token, char c);
void				handle_signal(void);
void				handle_ctrl_d(void);
void				handle_ctrl_c(void);
int					expand(Token *tokens);
t_cmd				*analyse_tokens(Token **tokens);
void				handle_ctrl_c(void);
void				handle_ctrl_d(void);
int					ft_is_separator(char c);
void				print_cmd(t_cmd *cmd);
int					is_red(Token *token);
int					get_red_type(Token *token);
int					nbr_argument(Token *tokens);
void				push_back(t_cmd **lst, t_cmd *node);
void				push_t_file(t_file **head, t_file *node);
void				free_token(Token *token);

///////////////////// execution /////////////////////////

typedef struct s_env
{
	char			*name;
	char			*vale;
	struct s_env	*next;
	struct s_env	*prv;
}					t_envi;

typedef struct s_pipe
{
	int				write_end;
	int				read_end;
}					t_pipe;

// typedef struct  s_exec
// {
//     char **args;
//     char

// }       t_exec;

// exit
typedef struct s_shell
{
	int				exit_status;
	char			**args;
}					t_shell;

typedef struct s_mini
{
	t_envi			*env;
	t_shell			*shell;
	char			**ptr;
	char			**arr;
}					t_mini;

int					builtins(char **av, t_mini *box);
int					is_builtin(char *cmd);
void				ft_putstr_fd(char *str, int fd);
int					ft_cd(char **ptr, t_envi *envi);
void				update_env(t_envi *envi);
t_envi				*search_env(t_envi *envi, char *name);
int					is_n_option(char *arg);
int					first_non_option(char **args);
int					ft_echo(char **args);
int					ft_unset(char **ptr, t_mini *box);
void				ft_remove(t_mini *box);
int					f__plus(char *r);
int					ft_export(char **ptr, t_envi *env);
int					ft_pwd(t_envi *env);
int					ft_exit(t_shell *shell);
int					ft_env(t_envi *env);

// extenal command
char				**separate_env(t_envi *env);
char				**get_path(void);
int					count_arguments(char **arguments);
int					handle_hd(char *delim, int expand, t_mini *box);
char				*expand_doc(char *s, t_mini *box);
ssize_t				calc_len(char *s, t_mini *box);
char				*expand_var(char *s, ssize_t *i, t_mini *box);
int					is_identifier(int c);
int					is_identifier(int c);

#endif