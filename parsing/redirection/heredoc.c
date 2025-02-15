/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 18:36:54 by alramire          #+#    #+#             */
/*   Updated: 2025/02/15 15:46:26 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

void	process_heredoc_input(t_redircmd *redir_node, t_scanner *scanner,
		t_list **heredoc_list)
{
	char	*heredoc_input;

	heredoc_input = collect_heredoc_input(redir_node->target, scanner->msh);
	if (!heredoc_input)
	{
		redir_node->heredoc_content = ft_strdup("");
		redir_node->heredoc_pid = -1;
		fprintf(stderr, "In handle_redir_heredoc: !heredoc_input\n");
	}
	append_node(heredoc_list, heredoc_input);
	free(heredoc_input);
	redir_node->heredoc_content = concatenate_lines(*heredoc_list);
}

void	update_redir_node_target(t_redircmd *redir_node, t_token *next_token)
{
	t_args	*file_args;

	file_args = OOM_GUARD(malloc(sizeof(t_args)), __FILE__, __LINE__);
	file_args->count = OOM_GUARD(malloc(sizeof(int)), __FILE__, __LINE__);
	*(file_args->count) = 1;
	args_collector(next_token, file_args);
	redir_node->target = ft_strdup(file_args->words[0]);
	free_args(&file_args);
	redir_node->target_type = determine_target_type(redir_node->target);
}

void	handle_redir_heredoc(t_redircmd *redir_node, t_scanner *scanner)
{
	t_list	*heredoc_list;

	heredoc_list = NULL;
	while (redir_node->redir_type == HEREDOC)
	{
		process_heredoc_input(redir_node, scanner, &heredoc_list);
		if (!scanner_has_next(scanner))
			break ;
		scanner->next = scanner_next(scanner);
		if (scanner->next.type != HEREDOC)
			break ;
		if (!scanner_has_next(scanner))
		{
			fprintf(stderr, "Syntax error: nothing after redirection token\n");
			break ;
		}
		scanner->next = scanner_next(scanner);
		update_redir_node_target(redir_node, &scanner->next);
	}
	free_list(heredoc_list);
}

/* void handle_redir_heredoc(t_redircmd *redir_node, t_scanner *scanner)
{
	t_list *heredoc_list = NULL;
	while (redir_node->redir_type == HEREDOC)
	{
		char *heredoc_input = collect_heredoc_input(redir_node->target,
				scanner->msh);
		if (!heredoc_input)
		{
			redir_node->heredoc_content = ft_strdup("");
			redir_node->heredoc_pid = -1;
			//free_args(&cmd_args);
			//cleanup(redir_node, EXIT_FAILURE);
			fprintf(stderr, "In handle_redir_heredoc: !heredoc_input\n");
		}
		append_node(&heredoc_list, heredoc_input);
		free(heredoc_input);
		redir_node->heredoc_content = concatenate_lines(heredoc_list);
		if (!scanner_has_next(scanner))
			break ;
		scanner->next = scanner_next(scanner);
		if (scanner->next.type != HEREDOC)
			break ;
		if (!scanner_has_next(scanner))
		{
			fprintf(stderr, "Syntax error: nothing after redirection token\n");
			break ;
		}
		scanner->next = scanner_next(scanner);
		t_args *file_args = OOM_GUARD(malloc(sizeof(t_args)), __FILE__,
				__LINE__);
		file_args->count = OOM_GUARD(malloc(sizeof(int)), __FILE__, __LINE__);
		*(file_args->count) = 1;
		args_collector(&scanner->next, file_args);
		redir_node->target = ft_strdup(file_args->words[0]);
		free_args(&file_args);
		redir_node->target_type = determine_target_type(redir_node->target);
	}
	free_list(heredoc_list);
} */
