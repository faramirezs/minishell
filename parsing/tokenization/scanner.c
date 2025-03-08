/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 19:46:40 by alramire          #+#    #+#             */
/*   Updated: 2025/03/08 19:13:45 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

t_scanner	scanner_value(t_char_itr char_itr)
{
	t_scanner	scanner;
	t_token		token;

	ft_memset(&token, 0, sizeof(t_token));
	scanner.char_itr = char_itr;
	scanner.next = token;
	scanner.next.lexeme.ptr = NULL;
	return (scanner);
}

int	scanner_has_next(t_scanner *self)
{
	char	c;

	skip_whitespaces(&self->char_itr);
	c = char_itr_peek(&self->char_itr);
	if (!(c == EOF || c == '\n' || c == '\0'))
	{
		return (!(c == EOF || c == '\n' || c == '\0'));
	}
	else
	{
		end_token((t_scanner *)self);
		return (!(c == EOF || c == '\n' || c == '\0'));
	}
}

t_token	scanner_next(t_scanner *self)
{
	skip_whitespaces(&self->char_itr);
	self->next.lexeme.length = 0;
	self->next = scanner_peek(self);
	return (self->next);
}

t_token	scanner_peek(t_scanner *self)
{
	while (1)
	{
		if (*self->char_itr.cursor == '|')
			return (pipe_token(self));
		else if (*self->char_itr.cursor == '>')
			return (redir_out_token(self));
		else if (*self->char_itr.cursor == '<')
			return (redir_in_token(self));
		else if (*self->char_itr.cursor == '\\')
			return (non_delimited_token(self));
		else if (*self->char_itr.cursor == '$' && self->next.type != HEREDOC)
			return (handle_expansions(self));
		else if (*self->char_itr.cursor == '/')
			return (abs_path_token(self));
		else if (*self->char_itr.cursor == '.' && ((*(self->char_itr.cursor
						+ 1) == '/') || (*(self->char_itr.cursor + 1) == '.'
					&& *(self->char_itr.cursor + 2) == '/')))
			return (rel_path_token(self));
		else if (ft_isalnum(*self->char_itr.cursor) || ft_strchr(NOBRKSYMS,
				*self->char_itr.cursor) || ft_strchr(QUOTEETC,
				*self->char_itr.cursor) || *self->char_itr.cursor == '$')
			return (non_delimited_token(self));
		else
			return (end_token(self));
	}
}
