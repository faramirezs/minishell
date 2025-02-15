/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 11:39:32 by alramire          #+#    #+#             */
/*   Updated: 2025/02/15 12:36:59 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARGS_H
# define ARGS_H

//# include "minishell.h"
//#include "char_itr.h"
# include "token.h"
//#include "scanner.h"

typedef struct s_args
{
	int		*count;
	char	**words;
}			t_args;

void		args_collector(t_token *token, t_args *args);
char		**copy_string_array(t_args *args);
void		free_string_array(char ***array);
void		free_args(t_args **args);
t_args		*initialize_args_count(void);

#endif
