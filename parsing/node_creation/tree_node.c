#include <stdlib.h>


#include "../../headers/tree_node.h"
#include "../../headers/guards.h"
#include "../../headers/scanner.h"
#include "../../headers/token.h"
#include "../../headers/minishell.h"
#include "../../headers/args.h"

//YouTube video min 7:45 https://youtu.be/sUxFE32tXF0?si=73UiqQEYAERD3fdD




t_tree_node *parse_tree_node (t_scanner *scanner)
{
	t_tree_node *node;

	node = OOM_GUARD(malloc(sizeof(t_tree_node)), __FILE__, __LINE__);
	if (scanner_has_next(scanner))
		scanner->next = scanner_next(scanner);
	printf("Next token type: %d, Next token value: %.*s\n", scanner->next.type, (int)scanner->next.lexeme.length, scanner->next.lexeme.start);
	//parse_redir
	/* if(scanner->next.type == REDIR_IN || scanner->next.type == REDIR_OUT || scanner->next.type == HEREDOC || scanner->next.type == APPEND_OUT)
		node = parse_redir; */
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

/* t_tree_node *parse_redir(t_scanner *scanner)
{
	t_tree_node *node;
	//Why we are declaring another poiner to tree_node? in parse_tree_node, we just declared and alloced the memory.
	t_args *args;

	node = OOM_GUARD(malloc(sizeof(t_tree_node)), __FILE__, __LINE__);
	args = OOM_GUARD(malloc(sizeof(t_args)), __FILE__, __LINE__);
	args->count = OOM_GUARD(malloc(sizeof(int)), __FILE__, __LINE__);
	node->type = N_REDIR;
	printf("Lexeme length: %li\n", scanner->next.lexeme.length);
	args_collector(&scanner->next, args);
	if (scanner_has_next(scanner))
		scanner_next(scanner);
	else
		printf("No file");

} */

t_tree_node *parse_exec(t_scanner *scanner)
{
	t_tree_node *node;
	//Why we are declaring another poiner to tree_node? in parse_tree_node, we just declared and alloced the memory.
	t_args *args;

	node = OOM_GUARD(malloc(sizeof(t_tree_node)), __FILE__, __LINE__);
	args = OOM_GUARD(malloc(sizeof(t_args)), __FILE__, __LINE__);
	args->count = OOM_GUARD(malloc(sizeof(int)), __FILE__, __LINE__);
	//after every arg_collection, we need to also parse redirs. That is the way bash does it.
	node->type = N_EXEC;
	printf("Lexeme length: %li\n", scanner->next.lexeme.length);
	//Lexeme includes null terminated char
	//node->data.exec_u.cmd = ft_strndup(scanner->next.lexeme.start, scanner->next.lexeme.length);
	//printf("Node type: %d\n", node->type);
	//printf("Node command: %s\n", node->data.exec_u.cmd);
	*(args->count) = 1;
	args_collector(&scanner->next, args);
	while(scanner_has_next(scanner))
	{
		(*(args->count))++;
		scanner_next(scanner);
		args_collector(&scanner->next, args);
	}
	node->data.exec_u.args = args->words;
	/* if (scanner_has_next(scanner))
	{
		//printf("Scanner has next\n");
		peek_token = scanner_next(scanner);
		//printf("peek token type %i\n", peek_token.type);
		printf("Peek token Type: %d, Peek token Value: %.*s\n", peek_token.type, (int)peek_token.lexeme.length, peek_token.lexeme.start);
		//printf("Token Type: %d, Token Value: %s\n", peek_token.type, new_token->value);
		if(peek_token.type == WORD || peek_token.type == OPTION)
		{
			//next_token = scanner_next(scanner);
			//printf("Next token Type: %d, Next token Value: %.*s\n", next_token.type, (int)next_token.lexeme.length, next_token.lexeme.start);
			//length = length + peek_token.lexeme.length;
			node->data.exec_u.args = ft_strndup(peek_token.lexeme.start, peek_token.lexeme.length);
			printf("Args, con ft_strndup is %s\n", node->data.exec_u.args);
			//node->data.exec_u.args = OOM_GUARD(malloc(sizeof(char) * length + 1), __FILE__, __LINE__);
		}
		else if (scanner_peek(scanner).type == PIPE)
		{
			printf("Is a Pipe, Args NULL\n");
			node->data.exec_u.args = NULL;
			parse_pipe(node, scanner);
		}
		else
		{
			printf("Args NULL, no pipe, no word, no option\n");
			node->data.exec_u.args = NULL;
		}
	}
	else
	{
		printf("Args NULL, scanner has no next.\n");
		node->data.exec_u.args = NULL;
	} */
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

// Take a look here https://www.youtube.com/watch?v=-J-nX4aiSP4&list=PLKUb7MEve0TjHQSKUWChAWyJPCpYMRovO&index=64
// and here https://www.youtube.com/watch?v=KbhDPYHRqkY&list=PLKUb7MEve0TjHQSKUWChAWyJPCpYMRovO&index=65
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
		printf("ARGS: args[0]=%s, args[1]=%s \n", node->data.exec_u.args[0], node->data.exec_u.args[1]);

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
