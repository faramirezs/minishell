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

static void	handle_quotes_and_escape(t_scanner *self, char **temp)
{
	if (*self->char_itr.cursor == '"')
		*temp = ft_strjoin_free_s1(*temp,
				double_quote_token(self).lexeme.start);
	else if (*self->char_itr.cursor == '\'')
		*temp = ft_strjoin_free_s1(*temp,
				single_quote_token(self).lexeme.start);
	else if (*self->char_itr.cursor == '\\'
		&& *(self->char_itr.cursor + 1) == '$')
	{
		*temp = ft_strjoin_free_s1(*temp, ft_strdup("$"));
		self->char_itr.cursor += 2;
	}
}

static void	append_normal_char(t_scanner *self, char **temp)
{
	char	*new_char;

	if (self->char_itr.cursor && *self->char_itr.cursor)
	{
		new_char = ft_substr(self->char_itr.cursor, 0, 1);
		if (new_char)
		{
			*temp = ft_strjoin_free_s1(*temp, new_char);
			self->char_itr.cursor++;
			free(new_char);
		}
	}
}
//static void	append_normal_char(t_scanner *self, char **temp)
//{
//	*temp = ft_strjoin_free_s1(*temp, ft_substr(self->char_itr.cursor, 0, 1));
//	self->char_itr.cursor++;
//}

t_token	non_delimited_token(t_scanner *self)
{
	char	*temp;

	temp = ft_strdup("");
	while (*self->char_itr.cursor
		&& !ft_strchr(" \t\n|><", *self->char_itr.cursor))
	{
		if (*self->char_itr.cursor == '"' || *self->char_itr.cursor == '\''
			|| (*self->char_itr.cursor == '\\'
				&& *(self->char_itr.cursor + 1) == '$'))
			handle_quotes_and_escape(self, &temp);
		else
			append_normal_char(self, &temp);
	}
	self->next.type = WORD;
	self->next.lexeme.start = temp;
	self->next.lexeme.length = ft_strlen(temp);
	return (self->next);
}
