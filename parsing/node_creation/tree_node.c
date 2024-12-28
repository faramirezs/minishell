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
	t_tree_node	*node;
	t_args		*args;
	int			pipe_flag;

	pipe_flag = 0;
	node = OOM_GUARD(malloc(sizeof(t_tree_node)), __FILE__, __LINE__);
	args = OOM_GUARD(malloc(sizeof(t_args)), __FILE__, __LINE__);
	args->count = OOM_GUARD(malloc(sizeof(int)), __FILE__, __LINE__);
	//printf("Next token type: %d, Next token value: %.*s\n", scanner->next.type, (int)scanner->next.lexeme.length, scanner->next.lexeme.start);
	//Logic when we call again the parse function and the first token is a PIPE
	//This can happen in two cases: 1 when the user input starts with PIPE, this is not valid (we should handle this somewhere else)
	//Or when before the pipe, we already collect arguments for the pipe left node
	scanner->next = scanner_next(scanner);
	//what happend if the token is not valid? or if the input is all whitespaces?
	*(args->count) = 1;
	args_collector(&scanner->next, args);
	if(scanner_has_next(scanner))
	{
		while(scanner->next.type != PIPE && scanner_has_next(scanner))
			{
				scanner->next = scanner_next(scanner);
				if(scanner->next.type == PIPE)
				{
					pipe_flag++;
					break;
				}
				(*(args->count))++;
				args_collector(&scanner->next, args);
				//parse redir
			}
		if(pipe_flag > 0)
			{
				if(scanner_has_next(scanner))
				{
					//scanner->next = scanner_next(scanner); //skip pipe token
					printf("Node type PIPE\n");
					pipe_flag = 0;
					return(parse_pipe(scanner, args));
				}
				else
				{
					printf("No arguments after pipe\n");
					return(node);
				}
			}
		else //pipe_flag = 0;
			{
				printf("Node type EXEC\n");
				return(parse_exec(args));
			}
	}
	else
	{
		//printf("++++Node type EXEC just one command++++\n");
		//check_null_array(args->words);
		//print_args(args);
		return(parse_exec(args));
	}

}
t_tree_node *parse_exec(t_args *args)
{
	t_tree_node *node;

	node = OOM_GUARD(malloc(sizeof(t_tree_node)), __FILE__, __LINE__);
	//args = OOM_GUARD(malloc(sizeof(t_args)), __FILE__, __LINE__);
	//args->count = OOM_GUARD(malloc(sizeof(int)), __FILE__, __LINE__);
	node->type = N_EXEC;
	//printf("++++parse_exec++++\n");
	//print_args(args);
	node->data.exec_u.args = copy_string_array(args);
	//print_array(node->data.exec_u.args);
	//print_args(args);
	return(node);
}



t_tree_node *parse_pipe (t_scanner *scanner, t_args *args)
{
	t_tree_node	*pipe_node;

	pipe_node = OOM_GUARD(malloc(sizeof(t_tree_node)), __FILE__, __LINE__);
	pipe_node->type = N_PIPE;
	pipe_node->data.pipe_u.left = parse_exec(args);
	printf("Left args\n");
	pipe_node->data.pipe_u.right = parse_tree_node(scanner);
	printf("Right args\n");
	return (pipe_node);
}


void visit_node (const t_tree_node *node, size_t spaces)
{
	indent_node(spaces);
	if(node->type == N_PIPE)
	{
		printf("Pipe node\n");
		visit_node(node->data.pipe_u.left, spaces + 2);
		visit_node(node->data.pipe_u.right, spaces + 2);
	}
	else if(node->type == N_EXEC)
	{
		printf("Exec node\n");
		//print_args(node->data.exec_u.args);

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



