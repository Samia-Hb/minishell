CFLAGS = -Wall -Wextra -Werror 

CC = cc -fsanitize=address -g3
LFLAGS = -lreadline
NAME = minishell

# Use wildcard to gather all .c files from libft and libftt directories
SRCS  = $(wildcard externel_folder/libftt/*.c) \
        $(wildcard externel_folder/gnl/*.c) \
		$(wildcard parsing/tokens/*.c) \
        $(wildcard parsing/parser/*.c) \
        $(wildcard parsing/signal/*.c) \
        $(wildcard parsing/abstract_sytax_tree/*.c) \
        $(wildcard execution/*.c) \
        execution/builtins/cd/cd.c \
		execution/builtins/echo/echo.c \
       	execution/builtins/env/env.c \
        execution/builtins/exit/exit.c \
        execution/builtins/pwd/pwd.c \
        execution/builtins/unset/unset.c \
        execution/builtins/builtins.c \
	  	execution/builtins/export/export.c \
        mnsh.c \


# Generate object file names from source file names
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all
