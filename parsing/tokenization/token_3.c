/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mestefan <mestefan@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 21:24:58 by mestefan          #+#    #+#             */
/*   Updated: 2025/02/15 21:26:30 by mestefan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

char	*expand_special_vars(char c, t_context *msh)
{
	if (c == '$')
		return (ft_itoa(getpid()));
	if (c == '?')
		return (ft_itoa(msh->ret_exit));
	return (NULL);
}

char	*expand_normal_var(t_scanner *self)
{
	char	var_name[256];
	int		i;
	char	*value;

	i = 0;
	while (ft_isalnum(*self->char_itr.cursor) || *self->char_itr.cursor == '_')
	{
		if (i < 255)
			var_name[i++] = *self->char_itr.cursor;
		else
			break ;
		self->char_itr.cursor++;
	}
	var_name[i] = '\0';
	value = ms_get_env(self->msh->env, var_name);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}

static char	*get_expansion_value(t_scanner *self)
{
	char	*value;

	value = expand_special_vars(*self->char_itr.cursor, self->msh);
	if (value)
		self->char_itr.cursor++;
	else
		value = expand_normal_var(self);
	return (value);
}

static char	*append_suffix(t_scanner *self, char *value)
{
	char	*suffix;
	char	*temp;

	suffix = ft_substr(self->char_itr.cursor, 0,
			ft_strlen(self->char_itr.cursor));
	temp = ft_strjoin(value, suffix);
	free(value);
	free(suffix);
	self->char_itr.cursor += ft_strlen(self->char_itr.cursor);
	return (temp);
}

t_token	handle_expansions(t_scanner *self)
{
	t_token	token;
	char	*value;

	self->char_itr.cursor++;
	if (!ft_isalnum(*self->char_itr.cursor) && *self->char_itr.cursor != '?'
		&& *self->char_itr.cursor != '_')
		return ((token.type = WORD,
				token.lexeme.start = ft_strdup("$"),
				token.lexeme.length = 1, token));
	value = get_expansion_value(self);
	if (*self->char_itr.cursor && (ft_isalnum(*self->char_itr.cursor)
			|| *self->char_itr.cursor == '_'))
		value = append_suffix(self, value);
	return ((token.type = WORD,
			token.lexeme.start = value,
			token.lexeme.length = ft_strlen(value), token));
}
