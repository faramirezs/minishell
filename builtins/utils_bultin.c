/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bultin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mestefan <mestefan@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 01:09:32 by mestefan          #+#    #+#             */
/*   Updated: 2025/02/10 01:09:38 by mestefan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/built_in.h"
#include "../headers/minishell.h"

t_bld_in	*find_builtin(t_bld_in *head, t_tree_node *node)
{
	if (!head || !node || !node->data.exec_u.args || !node->data.exec_u.args[0])
		return (NULL);
	while (head != NULL)
	{
		if (strcmp (head->name, node->data.exec_u.args[0]) == 0)
			return (head);
		head = head->next;
	}
	return (NULL);
}

static char	**ms_allocate_new_matrix(int size, const char *new_line)
{
	char	**new_matrix;

	new_matrix = oom_guard(malloc(sizeof(char *) * (size + 2)),
			__FILE__, __LINE__);
	if (!new_matrix)
		return (NULL);
	new_matrix[size] = ft_strdup(new_line);
	new_matrix[size + 1] = NULL;
	return (new_matrix);
}

static void	ms_copy_matrix(char **new_matrix, char **matrix, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		new_matrix[i] = ft_strdup(matrix[i]);
		i++;
	}
}

char	**ms_matrix_add_line(char **matrix, const char *new_line)
{
	int		size;
	char	**new_matrix;

	if (!new_line)
	{
		fprintf(stderr, "Error: new_line cannot be NULL\n");
		return (NULL);
	}
	size = 0;
	while (matrix && matrix[size])
		size++;
	new_matrix = ms_allocate_new_matrix(size, new_line);
	if (!new_matrix)
		return (NULL);
	if (matrix)
	{
		ms_copy_matrix(new_matrix, matrix, size);
		ft_free_tab(matrix);
	}
	return (new_matrix);
}

//char	**ms_matrix_add_line(char **matrix, const char *new_line)
//{
//	int		i;
//	char	**new_matrix;
//
//	i = 0;
//	if (!new_line)
//	{
//		fprintf(stderr, "Error: new_line cannot be NULL\n");
//		return (NULL);
//	}
//	if (!matrix)
//	{
//		new_matrix = oom_guard(malloc(sizeof(char *) * 2), __FILE__, __LINE__);
//		new_matrix[0] = ft_strdup(new_line);
//		new_matrix[1] = NULL;
//		return (new_matrix);
//	}
//	while (matrix[i])
//		i++;
//	new_matrix = oom_guard(malloc(sizeof(char *) * (i + 2)),
//		__FILE__, __LINE__);
//	i = 0;
//	while (matrix[i])
//	{
//		new_matrix[i] = ft_strdup(matrix[i]);
//		i++;
//	}
//	new_matrix[i] = ft_strdup(new_line);
//	new_matrix[i + 1] = NULL;
//	ft_free_tab(matrix);
//	return (new_matrix);
//}
