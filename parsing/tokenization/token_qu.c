/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_qu.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mestefan <mestefan@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 21:21:11 by mestefan          #+#    #+#             */
/*   Updated: 2025/02/15 21:24:37 by mestefan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static void	handle_dquote_expansions(t_scanner *self, char **expanded)
{
	t_token	var;

	if (*self->char_itr.cursor == '$' && *(self->char_itr.cursor + 1)
		&& find_env_index(self->msh->env, self->char_itr.cursor + 1))
	{
		var = handle_expansions(self);
		*expanded = (*expanded, var.lexeme.start);
	}
	else
	{
		*expanded = (*expanded,
				ft_substr(self->char_itr.cursor, 0, 1));
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
	while (*self->char_itr.cursor && *self->char_itr.cursor != '"')
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
	return ((self->next.lexeme.start = expanded,
			self->next.lexeme.length = ft_strlen(expanded), self->next));
}

static void	handle_squote_readline(t_scanner *self, char **expanded)
{
	char	*continuation;
	char	*tmp;

	continuation = readline("quote> ");
	self->next.lexeme.start = ++self->char_itr.cursor;
    expanded = ft_strdup("");
	if (!continuation)
	{
		free(*expanded);
		self->msh->ret_exit = 2;
		self->next.type = UNKNOWN;
		fprintf(stderr, "unexpected EOF while looking for matching `'\n");
		return ;
	}
	*expanded = (*expanded, continuation);
	free(continuation);
}

t_token	single_quote_token(t_scanner *self)
{
	char	*expanded;

	self->next.type = STRING_S_QUOTES;
	self->next.lexeme.start = ++self->char_itr.cursor;
	expanded = ft_strdup("");
	while (*self->char_itr.cursor && *self->char_itr.cursor != '\'')
	{
		if (*self->char_itr.cursor == '\0')
		{
			handle_squote_readline(self, &expanded);
			if (self->next.type == UNKNOWN)
				return (self->next);
			continue ;
		}
		expanded = ft_strjoin_free_s1(expanded,
				ft_substr(self->char_itr.cursor, 0, 1));
		self->char_itr.cursor++;
	}
	self->char_itr.cursor++;
	return ((self->next.lexeme.start = expanded,
			self->next.lexeme.length = ft_strlen(expanded), self->next));
}
