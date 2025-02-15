/* ************************************************************************** */
/*																			*/
/*														:::		::::::::   */
/*   scanner.h											:+:	  :+:	:+:   */
/*													+:+ +:+			+:+	 */
/*   By: alramire <alramire@student.42.fr>			+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+			*/
/*   Created: 2025/01/10 17:18:38 by alramire			#+#	#+#			 */
/*   Updated: 2025/01/10 17:20:12 by alramire			###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#ifndef SCANNER_H
# define SCANNER_H

# include "char_itr.h"
# include "env_var.h"
# include "token.h"
# include "args.h"

typedef struct s_scanner
{
	t_char_itr	char_itr;
	char		char_next;
	t_token		next;
	bool		inside_single_quotes;
	t_context	*msh;
}				t_scanner;

t_scanner		scanner_value(t_char_itr char_itr);
int				scanner_has_next(const t_scanner *self);
t_token			scanner_peek(t_scanner *self);
t_token			scanner_next(t_scanner *self);
void			parse(t_scanner *scanner);
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
char			*get_env_vvalue(t_scanner *self);
t_token			handle_expansions(t_scanner *self);
t_token			non_delimited_token(t_scanner *self);
void			process_args(t_scanner *scanner, t_args *args);

#endif
