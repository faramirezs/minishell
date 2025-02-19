/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_cleanup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 19:05:53 by alramire          #+#    #+#             */
/*   Updated: 2025/02/08 19:06:13 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"


void	free_string_array(char ***array)
{
	int	i;

	if (array && *array)
	{
		i = 0;
		while ((*array)[i] != NULL)
		{
			free((*array)[i]);
			(*array)[i] = NULL;
			i++;
		}
		free((*array));
		(*array) = NULL;
	}
}

void	free_args(t_args **args)
{
	if (args && *args)
	{
		if ((*args)->words)
		{
			free_string_array(&(*args)->words);
		}
		if ((*args)->count)
		{
			free((*args)->count);
			(*args)->count = NULL;
		}
		free(*args);
		*args = NULL;
	}
}
