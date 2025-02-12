/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 18:10:49 by alramire          #+#    #+#             */
/*   Updated: 2025/02/15 18:11:12 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

int	compare_token(const t_token *token, const char *str)
{
	return (strncmp(token->lexeme.start, str, token->lexeme.length));
}

void	check_cmd(t_scanner *self)
{
	size_t	i;

	i = 0;
	while (i < self->next.lexeme.length)
	{
		i++;
	}
	if (compare_token(&self->next, "echo") == 0
			|| compare_token(&self->next, "unset") == 0
            || compare_token(&self->next, "cd") == 0
            || compare_token(&self->next, "pwd") == 0
            || compare_token(&self->next, "export") == 0
            || compare_token(&self->next, "env") == 0)
	{
		self->next.type = COMMAND;
	}
	else
	{
		self->next.type = WORD;
	}
}
