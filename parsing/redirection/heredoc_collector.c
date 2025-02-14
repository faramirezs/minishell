/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 18:18:13 by alramire          #+#    #+#             */
/*   Updated: 2025/02/14 18:34:13 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/env_var.h"

static void handle_heredoc_interrupt(t_list *head, const char *delimiter)
{
	(void)delimiter;
	if (is_heredoc_interrupted())
	{
		free_list(head);
	}
}

static char *read_and_expand_line(const char *delimiter, t_context *msh)
{
	char *line = readline("heredoc> ");
	if (!line || is_heredoc_interrupted())
	{
		if (!line)
			fprintf(stderr, "Minishell: warning: here-document at line 1 delimited by end-of-file (wanted `%s')\n", delimiter);
		free(line);
		return NULL;
	}
	if (ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		return NULL;
	}
	char *expanded_line = expand_env_vars(line, msh);
	free(line);
	return expanded_line;
}

char *collect_heredoc_input(const char *delimiter, t_context *msh)
{
	t_list *head = NULL;
	char *line;
	char *result;

	setup_heredoc_signals();
	reset_heredoc_interrupt();

	while (1)
	{
		line = read_and_expand_line(delimiter, msh);
		if (!line)
			break;
		append_node(&head, line);
	}

	handle_heredoc_interrupt(head, delimiter);
	result = concatenate_lines(head);
	free_list(head);
	return result;
}
