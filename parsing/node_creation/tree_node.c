#include <stdlib.h>


#include "../../headers/tree_node.h"
#include "../../headers/guards.h"
#include "../../headers/scanner.h"
#include "../../headers/token.h"
#include "../../headers/minishell.h"

//YouTube video min 7:45 https://youtu.be/sUxFE32tXF0?si=73UiqQEYAERD3fdD




t_tree_node *parse_tree_node (t_scanner *scanner)
{
	t_tree_node *node;
	//t_token token;
	//t_token next_token;
	//if scanner_has_next....

	node = OOM_GUARD(malloc(sizeof(t_tree_node)), __FILE__, __LINE__);
	scanner->next = scanner_next(scanner);
	node = parse_exec(scanner);
	return(node);
	/* if (scanner->next.type == COMMAND)
		return(parse_exec(scanner));
	else if (scanner->next.type == PIPE)
		return(parse_pipe(scanner)); */





	//if peek = cmd?
	//		parse_exec
	//else if peek = |
	//		parse_pipe
	//else if peek = redir
	// 		parse redir
}

t_tree_node *parse_exec(t_scanner *scanner)
{
	t_tree_node *node;
	t_token peek_token;
	//size_t length;
	//char *str1;
	//char *str2;

	//length = 0;
	//node = malloc(sizeof(t_node));
	node = OOM_GUARD(malloc(sizeof(t_tree_node)), __FILE__, __LINE__);
	node->type = N_EXEC;
	node->data.exec_u.cmd = ft_strndup(scanner->next.lexeme.start, scanner->next.lexeme.length);
	if (scanner_has_next(scanner))
	{
		if(scanner_peek(scanner).type == WORD || scanner_peek(scanner).type == OPTION)
		{
			peek_token = scanner_next(scanner);
			//length = length + peek_token.lexeme.length;
			node->data.exec_u.args = ft_strndup(peek_token.lexeme.start, peek_token.lexeme.length);
			//node->data.exec_u.args = OOM_GUARD(malloc(sizeof(char) * length + 1), __FILE__, __LINE__);
		}
		else if (scanner_peek(scanner).type == PIPE)
		{
			node->data.exec_u.args = NULL;
			parse_pipe(node, scanner);
		}
		else
			node->data.exec_u.args = NULL;
	}
	else
		node->data.exec_u.args = NULL;
	return (node);
}


/* t_tree_node parse_redir ()
{

} */

/* t_tree_node *error_node(const char *msg)
{
	t_tree_node	*node;

	node = OOM_GUARD(malloc(sizeof(t_tree_node)), __FILE__, __LINE__);
	node->type = ERROR_NODE;
	node->data.error = msg;
} */

t_tree_node *parse_pipe (t_tree_node *node, t_scanner *scanner)
{
	t_tree_node	*pipe_node;
	//t_token		*token;

	pipe_node = OOM_GUARD(malloc(sizeof(t_tree_node)), __FILE__, __LINE__);
	pipe_node->type = N_PIPE;
	pipe_node->data.pipe_u.left = node;
	pipe_node->data.pipe_u.right = parse_tree_node(scanner);
	return (pipe_node);

	// This function will be called by parse.
	// On the left side I will add the first cmd + args and on the right one the second cmd + args
	// How can I put on the left side what I got before executing the pipe?
}


/* t_tree_node *redir_node_new(char *file, char *efile, int mode, int fd)
{
	(void)file;
	(void)efile;
	(void)mode;
	(void)fd;
}

t_tree_node *pipe_node_new(t_tree_node *left, t_tree_node *right)
{
	(void)left;
	(void)right;
} */


// Video used to get until this point: https://www.youtube.com/watch?v=sUxFE32tXF0&t=995s&ab_channel=KrisJordan


void visit_node (const t_tree_node *node, size_t spaces)
{
	indent_node(spaces);
	if(node->type == N_PIPE)
	{
		printf("Pipe node\n");
		visit_node(node->data.pipe_u.left, spaces + 2);
		visit_node(node->data.pipe_u.left, spaces + 2);
	}
	else if(node->type == N_EXEC)
	{
		printf("Exec node\n");
		printf("CMD: %s\n", node->data.exec_u.cmd);
		printf("ARGS: %s\n", node->data.exec_u.args);
	}
	else if(node->type == N_REDIR)
	{

	}
}


void indent_node (size_t spaces)
{
	size_t i;
	i = 0;

	while (i < spaces)
	{
		ft_putchar_fd(' ', 1);
		i++;
	}
}
