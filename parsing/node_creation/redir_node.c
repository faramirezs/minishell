/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_node.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 17:35:15 by alramire          #+#    #+#             */
/*   Updated: 2025/02/15 16:47:56 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

void	finalize_redir_node(t_args *cmd_args, t_tree_node *redir_node)
{
	if (cmd_args && cmd_args->words != NULL
		&& redir_node->data.redir_u.cmd == NULL)
	{
		redir_node->data.redir_u.cmd = parse_exec(cmd_args);
	}
}

t_tree_node	*handle_redir_pipe(t_scanner *scanner, t_tree_node *first_redir,
		t_tree_node *redir_node, t_args *cmd_args)
{
	if (first_redir != NULL)
		return (handle_pipe(scanner, first_redir, redir_node, cmd_args));
	else
		return (handle_pipe(scanner, redir_node, redir_node, cmd_args));
}

t_tree_node	*create_redir_node(t_scanner *scanner)
{
	t_tree_node	*redir_node;

	redir_node = oom_guard(malloc(sizeof(t_tree_node)), __FILE__, __LINE__);
	redir_node->type = N_REDIR;
	redir_node->data.redir_u.cmd = NULL;
	redir_node->data.redir_u.redir_type = scanner->next.type;
	if (scanner->next.type == REDIR_IN || scanner->next.type == HEREDOC)
		redir_node->data.redir_u.source_fd = STDIN_FILENO;
	else
		redir_node->data.redir_u.source_fd = STDOUT_FILENO;
	redir_node->data.redir_u.flags = get_redir_flags(scanner->next.type);
	redir_node->data.redir_u.mode = 0644;
	return (redir_node);
}

t_tree_node	*parse_redir_loop(t_scanner *scanner, t_args *cmd_args,
		t_tree_node *redir_node, t_tree_node *first_redir)
{
	t_tree_node	*node;

	node = NULL;
	while (scanner_has_next(scanner))
	{
		if (scanner->next.type == PIPE)
			return (handle_redir_pipe(scanner, first_redir, redir_node,
					cmd_args));
		scanner->next = scanner_next(scanner);
		if (check_redir(scanner))
		{
			node = parse_redir(scanner, cmd_args, redir_node);
			if (node->type == N_REDIR)
				redir_node->data.redir_u.cmd = node;
			else
				return (node);
		}
		else if (scanner->next.type == PIPE)
			return (handle_redir_pipe(scanner, first_redir, redir_node,
					cmd_args));
		else
			process_args(scanner, cmd_args);
	}
	finalize_redir_node(cmd_args, redir_node);
	return (redir_node);
}

t_tree_node	*parse_redir(t_scanner *scanner, t_args *cmd_args,
		t_tree_node *first_redir)
{
	t_tree_node	*redir_node;
	t_tree_node	*node;

	redir_node = create_redir_node(scanner);
	node = NULL;
	parse_redir_target(scanner, redir_node, initialize_args_count());
	return (parse_redir_loop(scanner, cmd_args, redir_node, first_redir));
}
