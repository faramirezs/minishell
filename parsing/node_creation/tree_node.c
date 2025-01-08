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
	//int			redir_i;

	pipe_flag = 0;
	//redir_i = 0;
	node = OOM_GUARD(malloc(sizeof(t_tree_node)), __FILE__, __LINE__);
	args = OOM_GUARD(malloc(sizeof(t_args)), __FILE__, __LINE__);
	args->count = OOM_GUARD(malloc(sizeof(int)), __FILE__, __LINE__);

	scanner->next = scanner_next(scanner);
	*(args->count) = 1;

	// Check if it's a redirection before collecting args
	if (scanner->next.type == REDIR_IN || scanner->next.type == REDIR_OUT ||
		scanner->next.type == APPEND_OUT || scanner->next.type == HEREDOC)
	{
		node->type = N_REDIR;
		return parse_redir(scanner);
	}

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

			// Check for redirections after initial args
			if (scanner->next.type == REDIR_IN || scanner->next.type == REDIR_OUT ||
				scanner->next.type == APPEND_OUT || scanner->next.type == HEREDOC)
			{
				node->type = N_REDIR;
				node->data.redir_u.args = parse_exec(args);  // Store current args as command
				return parse_redir(scanner);
			}

			(*(args->count))++;
			args_collector(&scanner->next, args);
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
		else
			{
				//printf("Node type EXEC\n");
				return(parse_exec(args));
			}
	}
	else
	{
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

t_tree_node *parse_redir (t_scanner *scanner)
{
	t_tree_node	*redir_node;
	t_args		*args;

	redir_node = OOM_GUARD(malloc(sizeof(t_tree_node)), __FILE__, __LINE__);
	redir_node->type = N_REDIR;
	args = OOM_GUARD(malloc(sizeof(t_args)), __FILE__, __LINE__);
	args->count = OOM_GUARD(malloc(sizeof(int)), __FILE__, __LINE__);
	redir_node->data.redir_u.redir_type = scanner->next.type;
	//*(args->count) = 1;
	//args_collector(&scanner->next, args);
	if(scanner_has_next(scanner))
	{
		scanner->next = scanner_next(scanner);
		*(args->count) = 1;
		args_collector(&scanner->next, args);
	}
	else
	{
		printf("Nothing after redir token/N");
	}
	/* pipe_node->data.pipe_u.left = parse_exec(args);
	printf("Left args\n");
	pipe_node->data.pipe_u.right = parse_tree_node(scanner);
	printf("Right args\n"); */
	return (redir_node);
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



