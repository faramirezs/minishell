/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocate_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 13:07:26 by jslusark          #+#    #+#             */
/*   Updated: 2024/11/25 13:14:43 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

void add_cmd_to_node(t_node *node_list, t_node *curr_node, t_token_list *token)
{
	t_cmd *cmd = calloc(1, sizeof(t_cmd));
	if (!cmd)
	{
		perror("Failed to allocate node\n");
		free_node_list(node_list); // Free the existing list in case of an error
		return;
	}
	cmd->cmd_type = token->type;
	cmd->cmd_value = ft_strdup(token->value); // coudl be issue when freeing the token list
	if (!cmd->cmd_value)
	{
		perror("Failed to allocate cmd_value\n");
		free(cmd);
		free_node_list(node_list); // Free the existing list in case of an error
		return; // if this happens it should stop so it has to return something!!!!!!!!!!!!
	}
	curr_node->cmd_data = cmd;
	// return(true);
}