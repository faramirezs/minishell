/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 18:18:13 by alramire          #+#    #+#             */
/*   Updated: 2025/01/16 19:09:55 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

typedef struct s_list
{
    char *line;
    struct s_list *next;
} t_list;

static t_list *create_node(const char *line)
{
    t_list *node = malloc(sizeof(t_list));
    if (!node)
        return NULL;
    node->line = strdup(line);
    node->next = NULL;
    return node;
}

static void append_node(t_list **head, const char *line)
{
    t_list *new_node = create_node(line);
    if (!new_node)
        return;
    if (!*head)
    {
        *head = new_node;
    }
    else
    {
        t_list *temp = *head;
        while (temp->next)
            temp = temp->next;
        temp->next = new_node;
    }
}

static size_t get_total_length(t_list *head)
{
    size_t total_length = 0;
    while (head)
    {
        total_length += strlen(head->line) + 1; // +1 for newline
        head = head->next;
    }
    return total_length;
}

static char *concatenate_lines(t_list *head)
{
    size_t total_length = get_total_length(head);
    char *result = malloc(total_length + 1); // +1 for null terminator
    if (!result)
        return NULL;

    char *ptr = result;
    while (head)
    {
        size_t len = strlen(head->line);
        strcpy(ptr, head->line);
        ptr += len;
        *ptr++ = '\n';
        head = head->next;
    }
    *ptr = '\0';
    return result;
}

static void free_list(t_list *head)
{
    while (head)
    {
        t_list *temp = head;
        head = head->next;
        free(temp->line);
        free(temp);
    }
}

char *collect_heredoc_input(const char *delimiter)
{
    t_list *head = NULL;
    char *line;
	char *result;

	setup_heredoc_signals();
    reset_heredoc_interrupt();

    while (1)
    {
        line = readline("heredoc> ");
        if (!line || is_heredoc_interrupted())
        {
            if (is_heredoc_interrupted())
                free(line);
            break;
        }

        if (ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }

        append_node(&head, line);
        free(line);
    }

	 if (is_heredoc_interrupted())
    {
        free_list(head);
        return NULL;
    }
    result = concatenate_lines(head);
    free_list(head);
    return result;
}
