/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 17:13:58 by alramire          #+#    #+#             */
/*   Updated: 2025/02/15 17:14:01 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

void	skip_whitespaces(t_char_itr *self)
{

	while (self->cursor && self->cursor < self->sentinel1
		&& ft_strchr(WHITESPACES, *self->cursor))
	{
		char_itr_next(self);
	}
}

int	is_whitespace(t_char_itr *self)
{
	return (self->cursor && self->cursor < self->sentinel1
		&& ft_strchr(WHITESPACES, *self->cursor));
}
