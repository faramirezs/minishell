/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 21:09:40 by mestefan          #+#    #+#             */
/*   Updated: 2025/03/02 15:33:36 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

t_token	new_token(t_token_type type, char *start, size_t length)
{
	t_token	token;

	token.type = type;
	token.lexeme.length = length;
	token.lexeme.start = start;
	return (token);
}

t_token	end_token(t_scanner *self)
{
	self->next.type = END;
	self->next.lexeme.length = 0;
	self->next.lexeme.start = self->char_itr.cursor;
	return (self->next);
}

t_token	pipe_token(t_scanner *self)
{
	self->next.type = PIPE;
	self->next.lexeme.length = 1;
	self->next.lexeme.start = self->char_itr.cursor;
	self->char_itr.cursor++;
	return (self->next);
}

t_token	redir_out_token(t_scanner *self)
{
	if (*(self->char_itr.cursor + 1) == '>')
		self->next = append_out_token(self);
	else
	{
		self->next.type = REDIR_OUT;
		self->next.lexeme.length = 1;
		self->next.lexeme.start = self->char_itr.cursor;
		self->char_itr.cursor++;
	}
	return (self->next);
}

t_token	redir_in_token(t_scanner *self)
{
	if (*(self->char_itr.cursor + 1) == '<')
		self->next = heredoc_token(self);
	else
	{
		self->next.type = REDIR_IN;
		self->next.lexeme.length = 1;
		self->next.lexeme.start = self->char_itr.cursor;
		self->char_itr.cursor++;
	}
	return (self->next);
}
