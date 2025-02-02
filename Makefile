NAME = minishell
LIBFT = libft/libft.a

TOKENIZATION =	./parsing/tokenization/char_itr.c \
				./parsing/tokenization/scanner.c \
				./parsing/tokenization/token.c \
				./parsing/tokenization/cmd.c \
				./parsing/tokenization/guards.c

NODE_CREATION =	./parsing/node_creation/tree_node.c \
				./parsing/node_creation/args.c \
				./parsing/redirection/redirections.c \
				./parsing/redirection/heredoc.c \
				./parsing/redirection/heredoc_signals.c

ENV_BLD	= ./env.c \
			./builtins/builtin/ft_cd.c \
			./builtins/builtin/ft_echo.c \
			./builtins/builtin/ft_env.c \
			./builtins/builtin/ft_exit.c \
			./builtins/builtin/ft_export.c \
			./builtins/builtin/ft_pwd.c \
			./builtins/builtin/ft_unset.c \
			./builtins/init/init_builtin.c \
			./builtins/init/init_env.c \
			./builtins/execute.c \
			./builtins/utils_bultin.c \

EXECUTION	= ./execution/exec.c

SIGNALS     = ./signal_handler.c

SRC	= main.c \
	$(TOKENIZATION) \
	$(NODE_CREATION) \
	$(EXECUTION) \
	$(ENV_BLD) \
    $(SIGNALS)

HEADERS	= ./headers/minishell.h \
			./headers/char_itr.h \
			./headers/guards.h \
			./headers/token.h \
			./headers/scanner.h \
			./headers/cmd.h \
			./headers/tree_node.h \
			./headers/args.h \
			./headers/built_in.h \
			./headers/env_var.h \
			./headers/redirections.h \
			./headers/signals.h

OBJS = $(SRC:.c=.o)

#CC	= clang -fsanitize=address
CC	= clang

RM	= rm -f
#CFLAGS	= -Wall -Wextra -Werror -g -I./include -I./libft -fsanitize=address
CFLAGS	= -Wall -Wextra -Werror -g -I./include -I./libft
PFLAGS = -lreadline

all:	$(LIBFT) $(NAME)

$(LIBFT):
		@make -C libft

$(NAME):	$(OBJS) $(HEADERS) $(LIBFT)
		$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) $(PFLAGS)

.c.o:
		$(CC) $(CFLAGS) -c $< -o $@

clean:
		$(RM) $(OBJS)
		@make -C libft clean

fclean:		clean
		$(RM) $(NAME)
		@make -C libft fclean

re:			fclean all

.PHONY:		all clean fclean re
