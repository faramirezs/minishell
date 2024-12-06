/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_nodes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 12:57:22 by jslusark          #+#    #+#             */
/*   Updated: 2024/11/26 16:27:32 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

void append_node(t_node **head, t_node *new_node)
{
	if(!(*head))
	{
		new_node->prev = NULL;
		*head = new_node; // This updates a local copy of head
	}
	else
	{
		t_node *last_node = *head; // we do this to traverse tthrough the list without affecting it
		while(last_node->next) // we traverse the list until we reach the node that has NULL as next node
			last_node = last_node->next;
		last_node->next = new_node; // we assign the new node as last in the list
		// last_node->next->next = NULL;
		new_node->prev = last_node; // we assign the 2nd last node as prev node of teh new node
	}
}

// t_node *create_node()
// {
// 	// t_node *new_node = calloc(1, sizeof(t_node));
// 	// if (!new_node) // i do  not free the node as i do it in the main
// 	// 	return(NULL);
// 	// return(new_node);
// }