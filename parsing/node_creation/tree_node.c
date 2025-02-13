/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_node.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 17:35:15 by alramire          #+#    #+#             */
/*   Updated: 2025/02/13 15:15:05 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/args.h"
#include "../../headers/minishell.h"

t_tree_node	*parse_tree_node(t_scanner *scanner)
{
	t_args	*args;

	args = OOM_GUARD(malloc(sizeof(t_args)), __FILE__, __LINE__);
	args->count = OOM_GUARD(malloc(sizeof(int)), __FILE__, __LINE__);
	scanner->next = scanner_next(scanner);
	*(args->count) = 0;
	if (check_redir(scanner))
	{
		return (parse_redir(scanner, args, NULL));
	}
	(*(args->count))++;
	args_collector(&scanner->next, args);
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
		(*(args->count))++;
		args_collector(&scanner->next, args);
	}
	return (parse_exec(args));
}

t_tree_node	*parse_exec(t_args *args)
{
	t_tree_node	*node;

	node = OOM_GUARD(malloc(sizeof(t_tree_node)), __FILE__, __LINE__);
	node->type = N_EXEC;
	node->data.exec_u.args = copy_string_array(args);
	return (node);
}

t_tree_node	*parse_pipe(t_scanner *scanner, t_args *args)
{
	t_tree_node	*pipe_node;

	pipe_node = OOM_GUARD(malloc(sizeof(t_tree_node)), __FILE__, __LINE__);
	pipe_node->type = N_PIPE;
	pipe_node->data.pipe_u.left = parse_exec(args);
	pipe_node->data.pipe_u.right = parse_tree_node(scanner);
	return (pipe_node);
}

void	free_tree_node(t_tree_node *node)
{
	if (!node)
		return ;
	if (node->type == N_REDIR)
	{
		if (node->data.redir_u.redir_type == HEREDOC)
		{
			cleanup_heredoc(&node->data.redir_u);
		}
		free(node->data.redir_u.target);
		node->data.redir_u.target = NULL;
		if (node->data.redir_u.cmd)
		{
			free_tree_node(node->data.redir_u.cmd);
			node->data.redir_u.cmd = NULL;
		}
	}
	else if (node->type == N_PIPE)
	{
		free_tree_node(node->data.pipe_u.left);
		free_tree_node(node->data.pipe_u.right);
	}
	else if (node->type == N_EXEC)
	{
		free_string_array(&node->data.exec_u.args);
	}
	free(node);
	node = NULL;
}
