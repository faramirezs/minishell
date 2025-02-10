/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_node.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 17:35:15 by alramire          #+#    #+#             */
/*   Updated: 2025/02/05 22:29:27 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

t_tree_node	*parse_tree_node(t_scanner *scanner)
{
	t_args	*args;

	args = initialize_args_count();
	scanner->next = scanner_next(scanner);
	if (check_redir(scanner))
	{
		return (parse_redir(scanner, args, NULL));
	}
	process_args(scanner, args);
	while (scanner_has_next(scanner))
	{
		scanner->next = scanner_next(scanner);
		if (check_redir(scanner))
		{
			return (parse_redir(scanner, args, NULL));
		}
		else if (scanner->next.type == PIPE)
		{
			return (parse_pipe(scanner, args));
		}
		process_args(scanner, args);
	}
	return (parse_exec(args));
}

t_tree_node	*parse_exec(t_args *args)
{
	t_tree_node	*node;

	node = oom_guard(malloc(sizeof(t_tree_node)), __FILE__, __LINE__);
	node->type = N_EXEC;
	node->data.exec_u.args = copy_string_array(args);
	return (node);
}

t_tree_node	*parse_pipe(t_scanner *scanner, t_args *args)
{
	t_tree_node	*pipe_node;

	pipe_node = oom_guard(malloc(sizeof(t_tree_node)), __FILE__, __LINE__);
	pipe_node->type = N_PIPE;
	pipe_node->data.pipe_u.left = parse_exec(args);
	pipe_node->data.pipe_u.right = parse_tree_node(scanner);
	return (pipe_node);
}
