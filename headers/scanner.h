/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   scanner.h										  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: alramire <alramire@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2025/01/10 17:18:38 by alramire		  #+#	#+#			 */
/*   Updated: 2025/01/10 17:20:12 by alramire		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */


#ifndef SCANNER_H
# define SCANNER_H

# include "token.h"
# include "char_itr.h"
# include "env_var.h"


/*
The scanner is like an interator
Char itr is producing a char at a time, the scanner is producing a token at a time
It looks like the mock token linked list? Can we create a linked list here?
 */

typedef struct s_scanner
{
	t_char_itr	char_itr;
	char		char_next;
	t_token		next;
	t_context	*msh;
}				t_scanner;

// Initializing the scanner struct.
t_scanner		scanner_value(t_char_itr char_itr);

// Returns 1 if there is more tokens to read, and 0 is not.
int				scanner_has_next(const t_scanner *self);

// This returns the token without advancind the iterator.
t_token 		scanner_peek(t_scanner *self);

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
t_token			single_quote_token(t_scanner *self);
t_token			double_quote_token(t_scanner *self);
char 			*get_env_vvalue(t_scanner *self);
t_slice 		expand_env_var(t_scanner *self);
char			*handle_expansions(const char *arg, t_context *msh);

#endif
