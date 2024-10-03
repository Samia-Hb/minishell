CFLAGS = -Wall -Wextra -Werror #-fsanitize=address -g3
CC = cc
LFLAGS = -lreadline
NAME = minishell

SRCS =  mnsh.c parsing/main.c parsing/analyse_tokens.c parsing/expand.c \
		parsing/token_into_stack.c parsing/signal_handle.c \
		parsing/postfix_to_tree.c parsing/minishell_utils.c \
		parsing/tokenization.c parsing/infix_to_postfix.c \
		parsing/pipex_utils.c parsing/parser.c \
		execution/libftt/ft_strjoin.c execution/libftt/ft_split.c execution/libftt/ft_strtrim.c \
		execution/libftt/ft_strdup.c execution/libftt/ft_strlen.c execution/libftt/ft_strlcpy.c \
		parsing/gnl/get_next_line.c parsing/gnl/get_next_line_utils.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LFLAGS)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: clean all
# execution/builtins/cd/cd.c execution/builtins/echo/echo.c execution/builtins/env/env.c \
# execution/builtins/exit/exit.c execution/builtins/pwd/pwd.c execution/builtins/unset/unset.c \
# execution/builtins/builtins.c execution/exec/commands.c execution/exec/ft_utiles.c \
# libft/*.c