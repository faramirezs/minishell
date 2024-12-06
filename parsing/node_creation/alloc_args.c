/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 13:08:41 by jslusark          #+#    #+#             */
/*   Updated: 2024/11/25 13:25:40 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

void append_newarg_to_cmdargs(t_args **cmd_args, t_args *new_arg)
{
	if (!(*cmd_args)) {
		*cmd_args = new_arg; // Update the actual pointer to the first argument
		new_arg->prev = NULL;
	} else {
		t_args *last_arg = *cmd_args; // Traverse the list to find the last argument
		while (last_arg->next) {
			last_arg = last_arg->next;
		}
		last_arg->next = new_arg; // Append the new argument to the list
		new_arg->prev = last_arg; // Set the previous pointer
	}
}

t_args *create_newarg_data(t_node *node_list, t_token_list *token)
{
	t_args *new_arg = calloc(1, sizeof(t_args));
	if (!new_arg)
	{
		perror("Failed to allocate node\n");
		free_node_list(node_list); // Free the existing list in case of an error
		return(NULL);
	}
	new_arg->arg_type = token->type;
	new_arg->arg_value = ft_strdup(token->value); // coudl be issue when freeing the token list
	if (!new_arg->arg_value)
	{
		perror("Failed to allocate arg_value\n");
		free(new_arg);
		free_node_list(node_list); // Free the existing list in case of an error
		return (NULL); // if this happens it should stop so it has to return something!!!!!!!!!!!!
	}
	return(new_arg);
}

void add_cmdargs_to_node(t_node *node_list, t_node *curr_node, t_args *head_arg)
{
	// t_args *head_arg = calloc(1, sizeof(t_args));
	if (!head_arg)
	{
		perror("Failed to allocate cmd_args\n");
		free_node_list(node_list); // Free the existing list in case of an error
		return;
	}
	// cmd->cmd_type = token->type;
	// cmd->cmd_value = ft_strdup(token->value); // coudl be issue when freeing the token list
	// if (!cmd->cmd_value)
	// {
	// 	perror("Failed to allocate cmd_value\n");
	// 	free(cmd);
	// 	free_node_list(node_list); // Free the existing list in case of an error
	// 	return; // if this happens it should stop so it has to return something!!!!!!!!!!!!
	// }
	curr_node->cmd_args = head_arg;
	// return(true);
}