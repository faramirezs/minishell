//#include "../../headers/built_in.h"
#include "../../headers/minishell.h"

void add_builtin(bld_in **head, const char *name, int (*func)(struct s_tree_node *node, t_context *msh))
{
	bld_in  *new_node;

	new_node = malloc(sizeof(bld_in));
	if (!new_node)
	{
		perror ("malloc");
		return ;
	}
	new_node->name = strdup(name);
	if (!new_node->name)
	{
		free (new_node);
		perror ("strdup");
		return ;
	}
	new_node->func = func;
	new_node->next = *head;
	*head = new_node;
}

bld_in  *create_builtin_list(void)
{
	bld_in  *head;

	head = NULL;
	add_builtin(&head, "echo", handle_echo);
	add_builtin (&head, "cd", handle_cd);
	add_builtin (&head, "pwd", handle_pwd);
	add_builtin (&head, "export", handle_export);
//	add_builtin (&head, "unset", handle_unset);
	add_builtin (&head, "env", handle_env);
	add_builtin (&head, "exit", handle_exit);

	return (head);
}

void	free_builtin_list(bld_in *head)
{
	while (head != NULL)
	{
		bld_in  *tmp;

		tmp = head;
		head = head->next;
		free (tmp->name);
		free (tmp);
	}
}
