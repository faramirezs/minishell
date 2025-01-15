/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandroramirez <alejandroramirez@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 17:18:38 by alramire          #+#    #+#             */
/*   Updated: 2025/01/15 21:29:33 by alejandrora      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
The scanner is where: input lexemes are transformed
into meaningful tokens. Scanner creates meaningful groups of tokens.
clasifying nodes.
*/

#ifndef TOKEN_ITR_H
# define TOKEN_ITR_H

# include "minishell.h"
# include "structs.h"


/*
The scanner is like an interator
Char itr is producing a char at a time, the scanner is producing a token at a time
It looks like the mock token linked list? Can we create a linked list here?
 */



// Initializing the scanner struct.
t_scanner		scanner_value(t_char_itr char_itr);

// Returns 1 if there is more tokens to read, and 0 is not.
int				scanner_has_next(const t_scanner *self);

// This returns the token without advancind the iterator.
t_token			scanner_peek(t_scanner *self);

// This returns the token advancing the iterator.
t_token			scanner_next(t_scanner *self);

// This functions returns the tokens. Not sure about this one.
void			parse(t_scanner *scanner);

// Token validation functions
t_token			end_token(t_scanner *self);
t_token			pipe_token(t_scanner *self);
t_token			redir_out_token(t_scanner *self);
t_token			redir_in_token(t_scanner *self);
t_token			word_token(t_scanner *self);
t_token			tmp_unknown_token(t_scanner *self);
t_token			env_var_token(t_scanner *self);
t_token			abs_path_token(t_scanner *self);
t_token			rel_path_token(t_scanner *self);
t_token			option_token(t_scanner *self);
t_token			append_out_token(t_scanner *self);
t_token			heredoc_token(t_scanner *self);
int				check_start_uknown(const char *input);

#endif
