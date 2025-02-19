#include <stdlib.h>
#include "garbage_collector.h"
#include "../headers/minishell.h"

void    gc_free_all(t_list *head)
{
    t_list  *curr;
    t_list  *next;

    curr = head;
    while (curr)
    {
        next = curr->next;
        if (curr->content)
            free(curr->content);
        free(curr);
        curr = next;
    }
}

void    gc_add_to_list(t_list **head, void *new)
{
    t_list  *node;
    t_list  *tmp;

    node = malloc(sizeof(t_list));
    if (!node)
    {
        ft_putstr_fd("Error: Failed to allocate memory\n", 2);
        return;
    }
    node->content = new;
    node->next = NULL;
    if (!(*head))
    {
        *head = node;
        return;
    }
    tmp = *head;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = node;
}

void    *gc_malloc(t_list **head, size_t size)
{
    void    *new;

    new = malloc(size);
    if (!new)
    {
        ft_putstr_fd("Error: Failed to allocate memory\n", 2);
        return (NULL);
    }
    gc_add_to_list(head, new);
    return (new);
}