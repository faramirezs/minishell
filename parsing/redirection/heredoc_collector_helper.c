/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_collector_helper.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 18:18:13 by alramire          #+#    #+#             */
/*   Updated: 2025/02/15 15:52:47 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static t_list	*create_node(const char *line)
{
	t_list	*node;

	node = malloc(sizeof(t_list));
	if (!node)
		return (NULL);
	node->line = strdup(line);
	node->next = NULL;
	return (node);
}

void	append_node(t_list **head, const char *line)
{
	t_list	*new_node;
	t_list	*temp;

	new_node = create_node(line);
	if (!new_node)
		return ;
	if (!*head)
	{
		*head = new_node;
	}
	else
	{
		temp = *head;
		while (temp->next)
			temp = temp->next;
		temp->next = new_node;
	}
}

static size_t	get_total_length(t_list *head)
{
	size_t	total_length;

	total_length = 0;
	while (head)
	{
		total_length += ft_strlen(head->line) + 1;
		head = head->next;
	}
	return (total_length);
}

char	*concatenate_lines(t_list *head)
{
	size_t	total_length;
	char	*result;
	char	*ptr;
	t_list	*current;
	size_t	len;

	total_length = get_total_length(head);
	result = malloc(total_length + 1);
	if (!result)
		return (NULL);
	ptr = result;
	current = head;
	while (current)
	{
		len = ft_strlen(current->line);
		ft_memcpy(ptr, current->line, len);
		ptr += len;
		*ptr++ = '\n';
		current = current->next;
	}
	*ptr = '\0';
	return (result);
}

void	free_list(t_list *head)
{
	t_list	*temp;

	while (head)
	{
		temp = head;
		head = head->next;
		free(temp->line);
		free(temp);
	}
}
