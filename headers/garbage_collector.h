#ifndef GARBAGE_COLLECTOR_H
# define GARBAGE_COLLECTOR_H

typedef struct s_list
{
    void            *content;
    struct s_list   *next;
}   t_list;

void    gc_free_all(t_list *head);
void    gc_add_to_list(t_list **head, void *new);
void    *gc_malloc(t_list **head, size_t size);

#endif