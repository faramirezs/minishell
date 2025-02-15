/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 16:59:18 by alramire          #+#    #+#             */
/*   Updated: 2025/02/15 16:59:20 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

# include <stddef.h>

typedef enum e_token_type // good to have an order like below
{
	COMMAND,
	ENV_VAR,
	ABS_PATH,
	REL_PATH,
	REDIR_IN,
	REDIR_OUT,
	APPEND_OUT,
	HEREDOC,
	PIPE,
	STRING_D_QUOTES,
	STRING_S_QUOTES,
	WORD,
	UNKNOWN,
	END,
}					t_token_type;

typedef struct s_slice
{
	const char		*start;
	size_t			length;
}					t_slice;

typedef struct s_token
{
	t_token_type	type;
	t_slice			lexeme;
}					t_token;

t_token				new_token(t_token_type type, char *start, size_t length);
void				print_token(const t_token token);
int					compare_token(const t_token *token, const char *str);

#endif
