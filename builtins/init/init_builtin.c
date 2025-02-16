/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mestefan <mestefan@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 01:34:58 by mestefan          #+#    #+#             */
/*   Updated: 2025/02/10 01:35:00 by mestefan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "../../headers/built_in.h"
#include "../../headers/minishell.h"

void	add_builtin(t_bld_in **head, const char *name,
			int (*func)(struct s_tree_node *node, t_context *msh))
{
	t_bld_in	*new_node;

	new_node = malloc(sizeof(t_bld_in));
	if (!new_node)
	{
		perror ("malloc");
		return ;
	}
	new_node->name = ft_strdup(name);
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

t_bld_in	*create_builtin_list(void)
{
	t_bld_in	*head;

	head = NULL;
	add_builtin(&head, "echo", handle_echo);
	add_builtin (&head, "cd", handle_cd);
	add_builtin (&head, "pwd", handle_pwd);
	add_builtin (&head, "export", handle_export);
	add_builtin (&head, "unset", handle_unset);
	add_builtin (&head, "env", handle_env);
	add_builtin (&head, "exit", handle_exit);
	return (head);
}

//void	free_builtin_list(t_bld_in *head)
//{
//	t_bld_in	*tmp;
//
//	while (head != NULL)
//	{
//		tmp = head;
//		head = head->next;
//		free (tmp->name);
//		free (tmp);
//	}
//	*head = NULL;
//}

void	free_builtin_list(t_bld_in **head)
{
	t_bld_in	*tmp;
	t_bld_in	*current;

	if (!head || !*head)
		return ;
	current = *head;
	while (current)
	{
		tmp = current->next;
		free(current->name);
		free(current);
		current = tmp;
	}
	*head = NULL;
}
