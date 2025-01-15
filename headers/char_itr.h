/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char_itr.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandroramirez <alejandroramirez@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 16:33:18 by alramire          #+#    #+#             */
/*   Updated: 2025/01/15 22:30:22 by alejandrora      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHAR_ITR_H
# define CHAR_ITR_H

# include "minishell.h"

# define WHITESPACES " \t\r\n\a\""
# define SYMBOLS "\\;()#&`*~|"
# define NOBRKSYMS "_.?:-+="



t_char_itr		char_itr_value(const char *star, size_t length);

const char		*char_itr_cursor(const t_char_itr *self);

int				char_itr_has_next(const t_char_itr *self);

char			char_itr_peek(const t_char_itr *self);

char			char_itr_next(t_char_itr *self);

void			skip_whitespaces(t_char_itr *self);

int				is_whitesace(t_char_itr *self);

#endif
