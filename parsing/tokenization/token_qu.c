/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_qu.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 21:21:11 by mestefan          #+#    #+#             */
/*   Updated: 2025/03/08 19:10:03 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static void	handle_dquote_expansions(t_scanner *self, char **expanded)
{
	t_token	var;
	char	*sub;

	if (*self->char_itr.cursor == '$' && *(self->char_itr.cursor + 1)
		&& find_env_index(self->msh->env, self->char_itr.cursor + 1))
	{
		var = handle_expansions(self);
		*expanded = ft_strjoin_free_s1(*expanded, var.lexeme.start);
		free(var.lexeme.ptr);
		var.lexeme.ptr = NULL;
	}
	else
	{
		sub = ft_substr(self->char_itr.cursor, 0, 1);
		*expanded = ft_strjoin_free_s1(*expanded, sub);
		free(sub);
		sub = NULL;
		self->char_itr.cursor++;
	}
}

void	handle_unexpected_eof(t_scanner *self, const char *expected)
{
	fprintf(stderr, "unexpected EOF while looking for `%s`\n", expected);
	self->msh->ret_exit = 2;
	self->next.type = UNKNOWN;
}

t_token	double_quote_token(t_scanner *self)
{
	char	*expanded;

	self->next.type = STRING_D_QUOTES;
	self->next.lexeme.start = ++self->char_itr.cursor;
	expanded = ft_strdup("");
	while ((*self->char_itr.cursor || *self->char_itr.cursor == '\0')
		&& *self->char_itr.cursor != '"')
	{
		if (*self->char_itr.cursor == '\0')
		{
			{
				free(expanded);
				handle_unexpected_eof(self, "\"");
				return (self->next);
			}
		}
		handle_dquote_expansions(self, &expanded);
	}
	self->char_itr.cursor++;
	self->next.lexeme.start = expanded;
	self->next.lexeme.ptr = expanded;
	self->next.lexeme.length = ft_strlen(expanded);
	return ((self->next));
}

t_token	single_quote_token(t_scanner *self)
{
	char	*expanded;
	char	*sub;

	self->next.type = STRING_S_QUOTES;
	self->next.lexeme.start = ++self->char_itr.cursor;
	expanded = ft_strdup("");
	while ((*self->char_itr.cursor || *self->char_itr.cursor == '\0') 
		&& *self->char_itr.cursor != '\'')
	{
		if (*self->char_itr.cursor == '\0')
		{
			free(expanded);
			handle_unexpected_eof(self, "\'");
			return (self->next);
		}
		sub = ft_substr(self->char_itr.cursor, 0, 1);
		expanded = ft_strjoin_free_s1(expanded, sub);
		free (sub);
		sub = NULL;
		self->char_itr.cursor++;
	}
	self->next.lexeme.start = expanded;
	self->next.lexeme.ptr = expanded;
	self->next.lexeme.length = ft_strlen(expanded);
	return (self->char_itr.cursor++, self->next);
}
