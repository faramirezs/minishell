/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   char_itr.c										 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: alramire <alramire@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2025/01/10 17:47:57 by alramire		  #+#	#+#			 */
/*   Updated: 2025/01/10 17:53:09 by alramire		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "../../headers/minishell.h"

t_char_itr	char_itr_value(const char *star, size_t length)
{
	t_char_itr	itr;

	itr.cursor = star;
	itr.sentinel1 = star + length;
	return (itr);
}
void	skip_whitespaces(t_char_itr *self)
{
	// char whitespaces[] = WHITESPACES;
	while (self->cursor && self->cursor < self->sentinel1
		&& ft_strchr(WHITESPACES, *self->cursor))
	{
		char_itr_next(self);
	}
}

int	is_whitespace(t_char_itr *self)
{
	// char whitespaces[] = WHITESPACES;
	return (self->cursor && self->cursor < self->sentinel1
		&& ft_strchr(WHITESPACES, *self->cursor));
}

int	char_itr_has_next(const t_char_itr *self)
{
	return (self->cursor < self->sentinel1);
}
char	char_itr_peek(const t_char_itr *self)
{
	if (char_itr_has_next(self))
	{
		return (*self->cursor);
	}
	else if (self->cursor == self->sentinel1)
	{
		return (*self->cursor);
	}
	else
	{
		fprintf(stderr, "%s:%d - Out of Bounds\n", __FILE__, __LINE__);
		exit(EXIT_FAILURE);
	}
}

char	char_itr_next(t_char_itr *self)
{
	if (char_itr_has_next(self))
	{
		return (*self->cursor++);
	}
	else
	{
		fprintf(stderr, "%s:%d - Out of Bounds\n", __FILE__, __LINE__);
		exit(EXIT_FAILURE);
	}
}

const char	*char_itr_cursor(const t_char_itr *self)
{
	return (self->cursor);
}
