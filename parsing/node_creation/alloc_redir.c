/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 13:08:44 by jslusark          #+#    #+#             */
/*   Updated: 2024/11/26 16:52:18 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

void append_redir_data(t_redir **head_redir, t_redir *curr_redir)
{
	if (!(*head_redir))
	{
		*head_redir = curr_redir; // Update the the redirection data as head redir
		curr_redir->prev = NULL;
	} else {
		t_redir *last_redir = *head_redir; // Traverse the redir list to find the last redir
		while (last_redir->next) {
			last_redir = last_redir->next;
		}
		last_redir->next = curr_redir; // Append the new argument to the list
		curr_redir->prev = last_redir; // Set the previous pointer
	}
}

t_redir *create_redir_data(t_token_list *token)
{
	t_redir *redir = calloc(1, sizeof(t_redir));
	if (!redir)
	{
		// perror("Failed to allocate node\n");
		// free_node_list(node_list); // Free the existing list in case of an error
		return NULL;
	}
	redir->redir_type = token->type;
	if(token->next != NULL)
	{
		redir->target = ft_strdup(token->next->value);
		redir->target_token_type = token->next->type;
		if(redir->redir_type == HEREDOC)
			redir->target_type = TARGET_DELIMITER;
		else
		{
			if(token->next->type == ABS_PATH || token->next->type == REL_PATH)
				redir->target_type = TARGET_PATHNAME; // after parsing we need to see if the target is an ENV_VAR
			else if(token->next->type == ENV_VAR)
				redir->target_type = TARGET_ENV_PATHNAME; // after parsing we need to see if the target is an ENV_VAR
			else
				redir->target_type = TARGET_FILENAME;
		}
	}
	return (redir);
	// curr_node->redir_data = redir;
}