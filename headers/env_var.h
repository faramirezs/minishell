/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 11:40:37 by alramire          #+#    #+#             */
/*   Updated: 2025/02/15 17:13:01 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_VAR_H
# define ENV_VAR_H

# include "minishell.h"
# include <limits.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdio.h>

typedef struct s_context
{
	int		fd[2];
	int		fd_close;
	char	**env;
	char	**env_export;
	int		ret_exit;
	char	*user;
}			t_context;

char		**ms_matrix_add_line(char **matrix, const char *new_line);
char		*ms_get_env(char **env, const char *key);
char		*ms_get_varenv(char **env, char *av);
int			env_compare(char **env, char **av);
int			ms_set_env(char **env, t_context *msh, const char *value);
char		**ms_remove_line(char **matrix, int index);
int			ms_unset_env(t_context *msh, const char *key);
int			find_env_index(char **env, const char *key);
void		free_env(char **env);
void		cleanup_context(t_context *msh);
t_context	*init_context(char **envp);
char		**duplicate_env(char **env);
void		check_shlvl(t_context *msh);

#endif
