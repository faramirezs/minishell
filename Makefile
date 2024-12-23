NAME = minishell
LIBFT = libft/libft.a

TOKENIZATION =	./parsing/tokenization/char_itr.c \
				./parsing/tokenization/scanner.c \
				./parsing/tokenization/token.c \
				./parsing/tokenization/cmd.c \
				./parsing/tokenization/token_list.c \
				./parsing/tokenization/guards.c

NODE_CREATION =	./parsing/node_creation/tree_node.c \
				./parsing/node_creation/args.c

EXECUTION	= ./execution/exec.c

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
		./headers/tree_node.h
#./headers/parser.h \


OBJS = $(SRC:.c=.o)

CC	= cc

RM	= rm -f

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
