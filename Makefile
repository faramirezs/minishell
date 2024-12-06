NAME = minishell
LIBFT = libft/libft.a

TOKENIZATION  = ./parsing/tokenization/mock_tokens.c \
				./parsing/tokenization/char_itr.c \
				./parsing/tokenization/scanner.c \
				./parsing/tokenization/token.c \
				./parsing/tokenization/cmd.c \
				./parsing/tokenization/token_list.c \
				./parsing/tokenization/guards.c

NODE_CREATION = 	./parsing/node_creation/return_nodelist.c \
					./parsing/node_creation/alloc_nodes.c \
					./parsing/node_creation/alloc_args.c \
					./parsing/node_creation/alloc_cmd.c \
					./parsing/node_creation/alloc_redir.c \
					./parsing/node_creation/free_nodes.c \
					./parsing/node_creation/print_nodes.c

EXECUTION	= ./execution/execution.c

SRC	= main.c \
	$(TOKENIZATION) \
	$(NODE_CREATION) \
	$(EXECUTION)

HEADERS	= ./headers/minishell.h \
		./headers/char_itr.h \
		./headers/guards.h \
		./headers/scanner.h \
		./headers/token.h \
		./headers/token_list.h \
		./headers/cmd.h \
		./headers/node.h \
#./headers/parser.h \


OBJS = $(SRC:.c=.o)

CC	= cc

RM	= rm -f

CFLAGS	= -Wall -Wextra -Werror -I./include -I./libft
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
