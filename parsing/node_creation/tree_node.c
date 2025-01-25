/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   tree_node.c										:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: alramire <alramire@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2025/01/10 17:35:15 by alramire		  #+#	#+#			 */
/*   Updated: 2025/01/17 13:13:32 by alramire		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

//#include "../../headers/tree_node.h"
//#include "../../headers/guards.h"
//#include "../../headers/scanner.h"
//#include "../../headers/token.h"
#include "../../headers/minishell.h"
//#include "../../headers/args.h"

//YouTube video min 7:45 https://youtu.be/sUxFE32tXF0?si=73UiqQEYAERD3fdD

t_tree_node *parse_tree_node (t_scanner *scanner)
{
	t_tree_node	*node = NULL;
	t_args		*args = NULL;
	int			pipe_flag;
	//int			redir_i;

	pipe_flag = 0;
	//redir_i = 0;

	node = OOM_GUARD(malloc(sizeof(t_tree_node)), __FILE__, __LINE__);
	args = OOM_GUARD(malloc(sizeof(t_args)), __FILE__, __LINE__);
	args->count = OOM_GUARD(malloc(sizeof(int)), __FILE__, __LINE__);

	*(args->count) = 1;
	scanner->next = scanner_next(scanner);

	// Check if it's a redirection before collecting args
	if (check_redir(scanner))
	{
	//	node->type = N_REDIR;
		free(args->count);
        free(args);
        free(node);
		return parse_redir(scanner, NULL);
	}

	args_collector(&scanner->next, args);

	while (scanner_has_next(scanner) && scanner->next.type != PIPE)
    {
        scanner->next = scanner_next(scanner);

        if (scanner->next.type == PIPE)
        {
            pipe_flag++;
            break;
        }

        // Check for redirection during argument collection
        if (check_redir(scanner))
        {
            free(args->count);
            free(args);
            free(node);
            return parse_redir(scanner, args);
        }

        (*(args->count))++;
        args_collector(&scanner->next, args);
    }

    if (pipe_flag > 0)
    {
        if (scanner_has_next(scanner))
        {
            free(node); // Node will be reallocated in parse_pipe
            return parse_pipe(scanner, args);
        }
        else
        {
            free(args->count);
            free(args);
            free(node);
            printf("No arguments after pipe\n");
            return NULL; // Avoid leaking memory on error
        }
    }

    // If no pipe is found, parse the collected arguments
    free(node); // Node will be reallocated in parse_exec
    return (parse_exec(args));
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
	//free(args->count);
    //free(args);
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
		printf("Redir node\n");
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

void free_tree_node(t_tree_node *node)
{
	if (!node)
		return;
	if (node->type == N_REDIR)
	{
		if (node->data.redir_u.redir_type == HEREDOC)
		{
			cleanup_heredoc(&node->data.redir_u);
		}
		free(node->data.redir_u.target);
		node->data.redir_u.target = NULL;
		free_tree_node(node->data.redir_u.cmd);
		node->data.redir_u.cmd = NULL;
	}
	else if (node->type == N_PIPE)
	{
		free_tree_node(node->data.pipe_u.left);
        node->data.pipe_u.left = NULL;

        free_tree_node(node->data.pipe_u.right);
        node->data.pipe_u.right = NULL;
	}
	else if (node->type == N_EXEC)
	{
		int i = 0;
		while (node->data.exec_u.args[i])
		{
			free(node->data.exec_u.args[i]);
			i++;
		}
		free(node->data.exec_u.args);
		//node->data.exec_u.args = NULL; do i need this?
	}

	free(node);
	//node = NULL; Do I need this?
}


