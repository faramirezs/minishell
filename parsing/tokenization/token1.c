/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mestefan <mestefan@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 21:12:02 by mestefan          #+#    #+#             */
/*   Updated: 2025/02/15 21:13:20 by mestefan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

// t_token	tmp_unknown_token(t_scanner *self)
// {
// 	self->next.type = UNKNOWN;
// 	self->next.lexeme.length = 1;
// 	self->next.lexeme.start = self->char_itr.cursor;
// 	self->char_itr.cursor++;
// 	self->next = scanner_peek(self);
// 	return (self->next);
// }
t_token tmp_unknown_token(t_scanner *self)
{
    self->next.type = UNKNOWN;
    self->next.lexeme.start = self->char_itr.cursor;
    self->next.lexeme.length = 0;  // Start with 0 and count up

    // Continue until we hit a delimiter or end
    while (self->char_itr.cursor && 
           *self->char_itr.cursor != ' ' && 
           *self->char_itr.cursor != '\t' && 
           *self->char_itr.cursor != '\n')
    {
        self->next.lexeme.length++;
        if (char_itr_has_next(&self->char_itr))
            self->char_itr.cursor++;
        else
            break;
    }

    return (self->next);
}

t_token	abs_path_token(t_scanner *self)
{
	self->next.type = ABS_PATH;
	self->next.lexeme.start = self->char_itr.cursor;
	while (self->char_itr.cursor && *self->char_itr.cursor != ' '
		&& *self->char_itr.cursor != '\t' && *self->char_itr.cursor != '\n')
	{
		self->next.lexeme.length++;
		if (char_itr_has_next(&self->char_itr) == 1)
			self->char_itr.cursor++;
		else
			break ;
	}
	return (self->next);
}

t_token	rel_path_token(t_scanner *self)
{
	self->next.type = REL_PATH;
	self->next.lexeme.start = self->char_itr.cursor;
	while (self->char_itr.cursor && *self->char_itr.cursor != ' '
		&& *self->char_itr.cursor != '\t' && *self->char_itr.cursor != '\n')
	{
		self->next.lexeme.length++;
		if (char_itr_has_next(&self->char_itr) == 1)
			self->char_itr.cursor++;
		else
			break ;
	}
	return (self->next);
}

t_token	append_out_token(t_scanner *self)
{
	self->next.type = APPEND_OUT;
	self->next.lexeme.length = 2;
	self->next.lexeme.start = self->char_itr.cursor;
	self->char_itr.cursor += 2;
	return (self->next);
}

t_token	heredoc_token(t_scanner *self)
{
	self->next.type = HEREDOC;
	self->next.lexeme.length = 2;
	self->next.lexeme.start = self->char_itr.cursor;
	self->char_itr.cursor += 2;
	return (self->next);
}
