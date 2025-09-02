/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 00:06:05 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/21 05:40:54 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "externel_folder/gnl/get_next_line.h"
# include "externel_folder/libftt/libft.h"
# include <dirent.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define SIGINT 2
# define SIGQUIT 3
# define SIGTERM 15

typedef enum t_TokenType
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
}								t_token_type;

typedef enum t_type
{
	PIPE,
	RE_OUT,
	RE_IN,
	RE_HEREDOC,
	RE_APPEND,
	UNKOWN
}								t_type;

typedef struct s_alst
{
	void						*content;
	struct s_alst				*next;
}								t_alst;

typedef struct s_env
{
	char						*name;
	char						*vale;
	struct s_env				*next;
	struct s_env				*prv;
}								t_envi;

typedef struct token
{
	t_token_type				type;
	char						*value;
	char						**expanded_value;
	struct token				*next;
	struct token				*previous;
}								t_token;

typedef struct s_file
{
	char						*filename;
	int							type;
	char						*red;
	struct s_file				*next;
}								t_file;

typedef struct s_cmd
{
	t_type						type;
	char						**arguments;
	t_file						*file;
	int							is_herdoc_end;
	struct s_cmd				*prev;
	struct s_cmd				*next;
	int							builtin;
	char						*cmd_path;
	int							pipe_fd[2];
	int							pid;
	int							in_fd;
	int							out_fd;
	int							stop;
}								t_cmd;

typedef struct s_var
{
	t_alst						*alist;
	int							exit_status;
	int							red_error;
	int							pre_pipe_infd;
	int							last_child_id;
	char						**envp;
	int							pipe_nb;
	int							size;
}								t_var;

typedef struct garbage_collector
{
	void						*ptr;
	struct garbage_collector	*next;
}								t_gc;

struct							s_global
{
	int							exit_status;
	int							pre_pipe_infd;
	int							last_child_id;
	int							out_fd;
	int							cpy_out_fd;
	int							cpy_in_fd;
	int							red_error;
	t_envi						*envp;
	int							num;
	int							size;
	int							pipe_nb;
	int							fd_here_doc;
	t_gc						*head;
	char						**en;
	t_alst						**alist;
	int							in_fd;
	int							flag;
	int							pid_size;
	int							*pid_table;
	int							interactive;
	int							*pid_array;
	int							i;
	int							status;
	int							stop;
	int							fd_heredoc;
};

extern struct s_global			*g_var;

int								file_expansion_null(char **filename);
int								lst_size(t_token *stack);
int								count_env_with_value(t_envi *env);
char							*generate_name(int *i);
t_token							**tokenize(char *input);
char							*handle_quote(char *str);
int								is_charactere(char c);
void							print_env(t_envi *envp);
void							case_function(char *input, char **result,
									int *j);
int								is_special(char c);
char							*handle_quote_v2(char *str);
int								check_quote(char *str);
int								is_number(char c);
int								one_dollar_test_case(int dollar_count,
									char *input, int *i);
int								dollar_counter(char *input);
void							exit_status_case(char *input, char **result,
									int *i, int *flag);
char							*single_quote_expansion(char *input, int *i);
char							*process_word(char *word);
char							*double_quote_expansion(char *input, int *i);
int								is_quoted(char *input);
char							*expand_non_operator(char *token);
int								built_in_checker(const char *str);
void							add_token(t_token **tokens, t_token_type type,
									char *value, int *k);
char							*get_executable(char *command);
char							*get_inside_quote(char *tmp, int *i, int *j);
char							*process_delimiter(char *tmp);
void							handle_heredoc(t_token **tokens, char *input,
									int *i);
void							heredoc_process(t_cmd **node, t_file **head,
									t_token **tokens);
char							*tidle_expansion(int *i);
void							fill_up_node(t_cmd **node, t_token **tokens,
									t_file **head);
char							*dollar_expand(char *input, int *i);
void							go_to_next(t_token **tokens);
char							**result_traitement(char *input);
char							*get_string(char *input, int *i);
int								get_size(char **arr);
int								get_size_arr(char *input);
char							*parse_line(char *input);
char							**handle_that_shit(char *input);
char							**unquoted_result(char **input);
char							*get_word_to_expand(char *str, int *j,
									char **result);
void							add_quote(char *input, char **expanded_value,
									int *j);
int								is_operator(t_token *node);
int								is_operand(t_token *node);
int								handle_consecutive_operator(t_token *tokens);
int								handle_paren(t_token *token);
int								handle_quotes(t_token *tokens);
int								check_token(char *str, char c);
t_token							*get_last_token(t_token *token);
int								handle_operators_bg_en(t_token *tokens);
int								check_syntax_errors(t_token *tokens);
char							quote_type(const char *str);
char							*char_to_string(char c, char c2);
int								get_token_type(const char *token, char c);
void							handle_signal(void);
void							handle_ctrl_c(void);
int								expand(t_token *tokens);
char							*ft_getenv(char *word);
t_cmd							*analyse_tokens(t_token **tokens);
void							handle_ctrl_c(void);
int								ft_is_separator(char c);
int								is_red(t_token *token);
int								get_red_type(t_token *token);
int								nbr_argument(t_token *tokens);
void							push_back(t_cmd **lst, t_cmd *node);
void							push_t_file(t_file **head, t_file *node);
void							*ft_malloc(int ele_nbr, size_t size);
void							clean_gc(void);
t_envi							*init_env(char **envp);
void							ctrl_c(int nb);
void							child_process(t_cmd *token, int btn);
void							add_node(void *data);
void							close_open_file(void);
char							*expand_heredoc(char *token);
char							*ft_strdup_1(const char *s1);
void							append_file_prep(char *path, int is_builtin);
void							exit_status(int status, char *filename, int fd);
int								handle_operator_expand(t_token **token);
void							check_heredoc_sign(int status, char *filename,
									int fd);
/////////////////////////////builtins///////////////////////////

void							ft_putstr_fd(char *str, int fd);
int								ft_cd(char **ptr, t_envi *envi);
void							update_env(t_envi *envi);
t_envi							*search_env(t_envi *envi, char *name);
int								is_n_option(char *arg);
int								first_non_option(char **args);
int								ft_echo(char **args);
int								ft_pwd(char **args);
void							ft_exit(char **args);
int								ft_unset(char **ptr);
void							ft_export(char **cmd);
int								ft_env(t_envi *env);
void							swap_nodes(t_envi *a, t_envi *b);
void							ft_remove(char **ptr, t_envi **envi, int i);
void							ft_free_array(char **array);
/////////////////////////////execution///////////////////////////
char							**create_env_array(t_envi *env, int count);
void							append_heredoc_prep(char *filename);
void							validate_cmd(t_cmd *cmd);
char							*allocate_folders(char *path, int i);
void							check_cmd_path(t_cmd *cmd);
void							execute_pipes(t_cmd *token, int pipe_nb,
									t_envi *env);
void							execute_arguments(t_cmd *token, t_envi *env);
void							exec_builtin(int btn, t_cmd *cmd, t_envi *envi);
void							execs(t_cmd *token, int btn, t_envi *env);
void							files_redirections(t_cmd *cmd, int builtin);
int								check_builtin(t_cmd *cmd);
int								count_commands(t_cmd *cmd);
void							close_files(t_cmd *token);
t_envi							*create__node(char *name, char *value);
void							add_env_node(t_envi **env_list,
									t_envi *new_node);
void							initiale_global(t_envi *env);
void							search_command_in_paths(t_cmd *cmd,
									char **path_dirs);
void							handle_command_not_found(t_cmd *cmd);
char							*construct_full_path(char *dir, char *cmd);
void							error_strdup(void);
void							shell_loop(t_envi *envi);
void							process_env_entry(char *env_entry,
									t_envi **env_list);
void							handle_file_redirections(t_cmd *cmd, int btn);
void							files_redirections(t_cmd *cmd, int builtin);
void							out_file_prep(char *path, int is_builtin);
void							in_file_prep(char *path, int is_builtin);
void							print_perror(char *str, int exitt);
void							check_cmd_path(t_cmd *token);
int								get_var_index(char *key);
void							check_command_name(t_cmd *token);
char							*allocate_folders(char *path, int i);
int								handle_stat_error(char *path, int is_builtin);
void							handle_file_redirections(t_cmd *token, int btn);
char							*put_cmd_status(int status, char *cmd_path,
									char *cmd);
char							*construct_full_path(char *dir, char *cmd);
void							handle_command_not_found(t_cmd *cmd);
void							search_command_in_paths(t_cmd *cmd,
									char **path_dirs);
void							check_command_name(t_cmd *cmd);
void							my_strncpy(char *dest, char *src, int n);
char							*get_cmd_path(char *cmd, char **dirs);
void							lista_add_front(t_alst **lst, t_alst *new);
t_alst							*lista_new(void *content);
char							**separate_env(t_envi *env);
void							parent_process(void);
void							close_file_descriptors(void);
void							ft_free_envp(t_envi *envp);
void							free_node_contents(t_envi *node);
void							update_existing_node(t_envi *current,
									const char *vale, int had_equals);
void							handle_invalid_identifier(const char *cmd);
void							process_cmd(t_envi **env, char **cmd, int i);
t_envi							*copy_list(t_envi *env);
void							update_or_add_env(t_envi **env, const char *var,
									const char *vale, int had_equals);
void							update_or_add_env_part1(t_envi **env,
									const char *var, t_envi **prev,
									t_envi **current);
void							free_node_contents(t_envi *node);
t_envi							*create_new_node(const char *var,
									const char *vale, int had_equals);
char							*set_node_vale(const char *vale);
void							update_existing_node(t_envi *current,
									const char *vale, int had_equals);
void							sync_env_array(t_envi *env);
void							handle_invalid_identifier(const char *cmd);
int								is_valid_identifier(const char *str);
void							free_env_array(char **env_array);
void							free_env(t_envi *env);
int								populate_env_array(char **env_array,
									t_envi *env, int count);
t_envi							*bubble_sort_env(t_envi *env);
void							print_export(t_envi *env);
void							handle_no_cmd(t_envi **env);
char							**env_to_array(t_envi *env);
void							sync_env_array(t_envi *env);
t_envi							*process_env_node(t_envi *current,
									t_envi **last_node);
char							*set_node_vale(const char *vale);
void							free_node_contents(t_envi *node);
void							update_existing_node(t_envi *current,
									const char *vale, int had_equals);
int								count_valid_env_entries(t_envi *env);
char							*create_env_string(t_envi *env_node);
int								is_valid_identifier(const char *str);
void							process_cmd(t_envi **env, char **cmd, int i);
void							handle_invalid_identifier(const char *cmd);
t_envi							*create_new_node(const char *var,
									const char *vale, int had_equals);
void							free_env(t_envi *env);
void							free_env_array(char **env_array);
t_envi							*add_new_var(const char *var, const char *vale,
									int had_equals);
void							update_or_add_env_part1(t_envi **env,
									const char *var, t_envi **prev,
									t_envi **current);
void							update_or_add_env(t_envi **env, const char *var,
									const char *vale, int had_equals);
int								is_numeric(char *str);
long							atoi_long(const char *str);
int								grr(char *str);
long							convert_to_long(const char *str, int sign);
int								handle_sign_and_whitespace(const char **str);
void							setup_signals_and_fork(void);
#endif