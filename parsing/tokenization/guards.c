/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   guards.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 16:30:26 by alramire          #+#    #+#             */
/*   Updated: 2025/02/15 16:48:06 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

void	*oom_guard(void *ptr, char *file, int number)
{
	if (ptr == NULL)
	{
		fprintf(stderr, "%s:%d Out of Memory", file, number);
		exit(EXIT_FAILURE);
	}
	return (ptr);
}
