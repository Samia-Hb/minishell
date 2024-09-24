CFLAGS = -Wall -Wextra -Werror
CC = cc
LFLAGS =-lreadline
NAME = minishell

SRCS =  mnsh.c parsing/main.c parsing/analyse_tokens.c parsing/expand.c parsing/analyse_tokens.c \
		parsing/token_into_stack.c parsing/signal_handle.c \
		parsing/postfix_to_tree.c parsing/minishell_utils.c \
		parsing/tokenization.c parsing/infix_to_postfix.c \
		parsing/pipex_utils.c parsing/parser.c \
		parsing/gnl/get_next_line.c parsing/gnl/get_next_line_utils.c \
		execution/builtins/cd execution/builtins/echo execution/builtins/env \
		execution/builtins/exit execution/builtins/pwd execution/builtins/unset \
		execution/builtins/builtins.c execution/exec/commands.c execution/exec/ft_utiles.c  
OBJS = $(SRCS:.c=.o)execution/libftt/*.c 

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