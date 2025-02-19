/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mestefan <mestefan@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 21:15:29 by mestefan          #+#    #+#             */
/*   Updated: 2025/02/15 21:19:06 by mestefan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

bool	escape_special_chars(t_scanner *self)
{
	if (*self->char_itr.cursor == '\\' && *(self->char_itr.cursor + 1) == '$')
	{
		self->char_itr.cursor++;
		return (true);
	}
	return (false);
}

static void	handle_quotes_and_escape(t_scanner *self)
{
	if (*self->char_itr.cursor == '"')
		self->next.lexeme.start = ft_strjoin_free_s1((char *)self->next.lexeme.start,
				double_quote_token(self).lexeme.start);
	else if (*self->char_itr.cursor == '\'')
		self->next.lexeme.start = ft_strjoin_free_s1((char *)self->next.lexeme.start,
				single_quote_token(self).lexeme.start);
	else if (*self->char_itr.cursor == '\\'
		&& *(self->char_itr.cursor + 1) == '$')
	{
		self->next.lexeme.start = ft_strjoin_free_s1((char *)self->next.lexeme.start, ft_strdup("$"));
		self->char_itr.cursor += 2;
	}
}

static void	append_normal_char(t_scanner *self)
{
	char	*new_char;

	if (self->char_itr.cursor && *self->char_itr.cursor)
	{
		new_char = ft_substr(self->char_itr.cursor, 0, 1);
		if (!new_char)
			return ;
		self->next.lexeme.start = ft_strjoin_free_s1((char *)self->next.lexeme.start, new_char);
		self->char_itr.cursor++;
		free(new_char);
	}
}
/**/
t_token	non_delimited_token(t_scanner *self)
{
	self->next.lexeme.start = ft_strdup("");
	while (*self->char_itr.cursor
		&& !ft_strchr(" \t\n|><", *self->char_itr.cursor))
	{
		if (*self->char_itr.cursor == '"' || *self->char_itr.cursor == '\''
			|| (*self->char_itr.cursor == '\\'
				&& *(self->char_itr.cursor + 1) == '$'))
			handle_quotes_and_escape(self);
		else
			append_normal_char(self);
	}
	self->next.type = WORD;
	self->next.lexeme.length = ft_strlen(self->next.lexeme.start);
	return (self->next);
}
