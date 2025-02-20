/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 17:13:58 by alramire          #+#    #+#             */
/*   Updated: 2025/02/15 18:11:53 by alramire         ###   ########.fr       */
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

// bool	is_cmd(t_scanner *self)
// {
// 	if ((self->next.lexeme.length == 4 && ft_strncmp(self->next.lexeme.start, "echo", 4) == 0)
// 		|| (self->next.lexeme.length == 2 && ft_strncmp(self->next.lexeme.start, "cd", 2) == 0)
// 		|| (self->next.lexeme.length == 3 && ft_strncmp(self->next.lexeme.start, "pwd", 3) == 0)
// 		|| (self->next.lexeme.length == 6 && ft_strncmp(self->next.lexeme.start, "export", 6) == 0)
// 		|| (self->next.lexeme.length == 5 && ft_strncmp(self->next.lexeme.start, "unset", 5) == 0)
// 		|| (self->next.lexeme.length == 3 && ft_strncmp(self->next.lexeme.start, "env", 3) == 0)
// 		|| (self->next.lexeme.length == 4 && ft_strncmp(self->next.lexeme.start, "exit", 4) == 0)
// 		|| (self->next.lexeme.length == 2 && ft_strncmp(self->next.lexeme.start, "ls", 2) == 0)
// 		|| (self->next.lexeme.length == 4 && ft_strncmp(self->next.lexeme.start, "grep", 4) == 0)
// 		|| (self->next.lexeme.length == 3 && ft_strncmp(self->next.lexeme.start, "cat", 3) == 0))
// 	{
// 		return (true);
// 	}
// 	return (false);
// }

// t_token	cmd_token(t_scanner *self)
// {
// 	self->next.lexeme.start = self->char_itr.cursor;
// 	while (*self->char_itr.cursor && !ft_strchr(" \t\n|><", *self->char_itr.cursor))
// 		self->char_itr.cursor++;
// 	self->next.lexeme.length = self->char_itr.cursor - self->next.lexeme.start;
// 	if (is_cmd(self))
// 		self->next.type = COMMAND;
// 	else
// 		self->next.type = WORD;
// 	return (self->next);
// }
