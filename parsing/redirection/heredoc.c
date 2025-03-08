/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 18:36:54 by alramire          #+#    #+#             */
/*   Updated: 2025/03/08 15:56:00 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

void	process_heredoc_input(t_redircmd *redir_node, t_scanner *scanner,
		t_list **heredoc_list)
{
	char	*heredoc_input;

	heredoc_input = collect_heredoc_input(redir_node->target, scanner->msh);
	if (is_heredoc_interrupted())
	{
		redir_node->heredoc_content = ft_strdup("");
		redir_node->heredoc_pid = -1;
		restore_global_signals();
		scanner->msh->ret_exit = 130;
		return ;
	}
	if (!heredoc_input)
	{
		redir_node->heredoc_content = ft_strdup("");
		redir_node->heredoc_pid = -1;
	}
	else
	{
		append_node(heredoc_list, heredoc_input);
		free(heredoc_input);
		redir_node->heredoc_content = concatenate_lines(*heredoc_list);
	}
}

void	update_redir_node_target(t_redircmd *redir_node, t_token *next_token)
{
	t_args	*file_args;

	file_args = oom_guard(malloc(sizeof(t_args)), __FILE__, __LINE__);
	file_args->count = oom_guard(malloc(sizeof(int)), __FILE__, __LINE__);
	*(file_args->count) = 1;
	args_collector(next_token, file_args);
	redir_node->target = ft_strdup(file_args->words[0]);
	free_args(&file_args);
	redir_node->target_type = determine_target_type(redir_node->target);
}

static int	process_next_heredoc_token(t_scanner *scanner)
{
	if (!scanner_has_next(scanner))
		return (0);
	scanner->next = scanner_next(scanner);
	if (scanner->next.type != HEREDOC)
	{
		free(scanner->next.lexeme.ptr);
		return (0);
	}
	if (!scanner_has_next(scanner))
	{
		fprintf(stderr, "Syntax error: nothing after redirection token\n");
		return (0);
	}
	return (1);
}

void	handle_redir_heredoc(t_redircmd *redir_node, t_scanner *scanner)
{
	t_list	*heredoc_list;

	heredoc_list = NULL;
	while (redir_node->redir_type == HEREDOC)
	{
		process_heredoc_input(redir_node, scanner, &heredoc_list);
		if (!process_next_heredoc_token(scanner))
			break ;
		scanner->next = scanner_next(scanner);
		free(redir_node->heredoc_content);
		if (redir_node->target)
		{
			free(redir_node->target);
			redir_node->target = NULL;
		}
		update_redir_node_target(redir_node, &scanner->next);
	}
	free_list(heredoc_list);
}
