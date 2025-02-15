/* ************************************************************************** */
/*																			*/
/*														:::		::::::::   */
/*   char_itr.h											:+:	  :+:	:+:   */
/*													+:+ +:+			+:+	 */
/*   By: alramire <alramire@student.42.fr>			+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+			*/
/*   Created: 2024/11/06 16:33:18 by alramire			#+#	#+#			 */
/*   Updated: 2025/01/10 17:08:15 by alramire			###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#ifndef CHAR_ITR_H
# define CHAR_ITR_H

# include "minishell.h"

//# include <stddef.h>

# define WHITESPACES " \t\r\n\a"
# define SYMBOLS "\\;()#&`*~|"
# define NOBRKSYMS "_.?:-+="
# define QUOTEETC "\"'"

typedef struct s_chart_itr
{
	const char	*cursor;
	const char	*sentinel1;
}				t_char_itr;

t_char_itr		char_itr_value(const char *star, size_t length);

const char		*char_itr_cursor(const t_char_itr *self);

int				char_itr_has_next(const t_char_itr *self);

char			char_itr_peek(const t_char_itr *self);

char			char_itr_next(t_char_itr *self);

void			skip_whitespaces(t_char_itr *self);

int				is_whitesace(t_char_itr *self);

#endif
