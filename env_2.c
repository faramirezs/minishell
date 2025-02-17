/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mestefan <mestefan@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 01:05:01 by mestefan          #+#    #+#             */
/*   Updated: 2025/02/16 01:06:13 by mestefan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/minishell.h"
#include "headers/env_var.h"

int	find_env_index(char **env, const char *key)
{
	int		i;
	size_t	key_len;
	char	*equals;

	i = 0;
	key_len = ft_strlen(key);
	while (env[i])
	{
		equals = ft_strchr(env[i], '=');
		if (equals && (size_t)(equals - env[i]) == key_len
			&& ft_strncmp(env[i], key, key_len) == 0)
			return (i);
		i++;
	}
	return (-1);
}

char	**allocate_new_matrix(int size)
{
	char	**new_matrix;

	new_matrix = malloc(sizeof(char *) * size);
	if (!new_matrix)
		return (NULL);
	return (new_matrix);
}

int	copy_matrix_except_index(char **matrix, char **new_matrix, int index)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (matrix[i])
	{
		if (i != index)
		{
			new_matrix[j] = ft_strdup(matrix[i]);
			if (!new_matrix[j])
			{
				ft_free_tab(new_matrix);
				return (0);
			}
			j++;
		}
		i++;
	}
	new_matrix[j] = NULL;
	return (1);
}

char	**ms_remove_line(char **matrix, int index)
{
	int		size;
	char	**new_matrix;

	size = 0;
	while (matrix[size])
		size++;
	new_matrix = allocate_new_matrix(size);
	if (!new_matrix)
		return (NULL);
	if (!copy_matrix_except_index(matrix, new_matrix, index))
		return (NULL);
	ft_free_tab(matrix);
	return (new_matrix);
}
// char	**ms_remove_line(char **matrix, int index)
// {
// 	int		i;
// 	int		j;
// 	int		size;
// 	char	**new_matrix;

// 	i = 0;
// 	j = 0;
// 	size = 0;
// 	while (matrix[size])
// 		size++;
// 	new_matrix = malloc(sizeof(char *) * size);
// 	if (!new_matrix)
// 		return (NULL);
// 	while (matrix[i])
// 	{
// 		if (i != index)
// 		{
// 			new_matrix[j] = ft_strdup(matrix[i]);
// 			if (!new_matrix[j])
// 			{
// 				ft_free_tab (new_matrix);
// 				return (NULL);
// 			}
// 			j++;
// 		}
// 		i++;
// 	}
// 	new_matrix[j] = NULL;
// 	ft_free_tab(matrix);
// 	return (new_matrix);
// }
