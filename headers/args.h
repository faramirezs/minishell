/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mestefan <mestefan@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 02:00:21 by mestefan          #+#    #+#             */
/*   Updated: 2025/02/10 02:00:25 by mestefan         ###   ########.fr       */
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
