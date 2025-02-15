/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 18:36:54 by alramire          #+#    #+#             */
/*   Updated: 2025/02/15 16:49:42 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

void	parse_redir_target(t_scanner *scanner, t_tree_node *redir_node,
		t_args *file_args)
{
	t_redircmd	*rcmd;

	rcmd = &redir_node->data.redir_u;
	if (!scanner_has_next(scanner))
	{
		printf("Syntax error: nothing after redirection token\n");
		cleanup(redir_node, EXIT_FAILURE);
	}
	scanner->next = scanner_next(scanner);
	*(file_args->count) = 1;
	args_collector(&scanner->next, file_args);
	rcmd->target = ft_strdup(file_args->words[0]);
	free_args(&file_args);
	rcmd->target_type = determine_target_type(rcmd->target);
	if (rcmd->redir_type == HEREDOC)
		handle_redir_heredoc(rcmd, scanner);
}

t_tree_node	*handle_pipe(t_scanner *scanner, t_tree_node *first_redir,
		t_tree_node *last_redir, t_args *cmd_args)
{
	t_tree_node	*pipe_node;

	pipe_node = oom_guard(malloc(sizeof(t_tree_node)), __FILE__, __LINE__);
	pipe_node->type = N_PIPE;
	if (cmd_args && cmd_args->words != NULL)
	{
		last_redir->data.redir_u.cmd = parse_exec(cmd_args);
		if (first_redir != last_redir)
			first_redir->data.redir_u.cmd = last_redir;
	}
	pipe_node->data.pipe_u.left = first_redir;
	pipe_node->data.pipe_u.right = parse_tree_node(scanner);
	return (pipe_node);
}

int	determine_target_type(const char *target)
{
	if (ft_strchr(target, '/'))
		return (TARGET_PATHNAME);
	if (target[0] == '$')
		return (TARGET_ENV_PATHNAME);
	return (TARGET_FILENAME);
}

int	get_redir_flags(int redir_type)
{
	if (redir_type == REDIR_IN)
		return (O_RDONLY);
	else if (redir_type == REDIR_OUT)
		return (O_WRONLY | O_CREAT | O_TRUNC);
	else if (redir_type == APPEND_OUT)
		return (O_WRONLY | O_CREAT | O_APPEND);
	else if (redir_type == REDIR_IN)
		return (O_RDONLY);
	else if (redir_type == REDIR_IN)
		return (O_RDONLY);
	else
		return (0);
}

int	check_redir(t_scanner *scanner)
{
	return ((scanner->next.type == REDIR_IN || scanner->next.type == REDIR_OUT
			|| scanner->next.type == APPEND_OUT
			|| scanner->next.type == HEREDOC));
}
