/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mestefan <mestefan@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 02:00:47 by mestefan          #+#    #+#             */
/*   Updated: 2025/02/10 02:00:50 by mestefan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILT_IN_H
# define BUILT_IN_H

# include "minishell.h"

typedef struct s_bld_in {
	struct s_bld_in	*next;
	char			*name;
	int				(*func)(t_tree_node *node, t_context *msh);
}	t_bld_in;

t_bld_in	*find_builtin(t_bld_in *head, t_tree_node *node);
t_bld_in	*create_builtin_list(void);
int			handle_cd(struct s_tree_node *node, t_context *msh);
int			handle_echo(struct s_tree_node *node, t_context *msh);
int			handle_exit(struct s_tree_node *node, t_context *msh);
int			handle_export(struct s_tree_node *node, t_context *msh);
int			handle_pwd(struct s_tree_node *node, t_context *msh);
int			handle_unset(struct s_tree_node *node, t_context *msh);
int			handle_env(struct s_tree_node *node, t_context *msh);
void		free_builtin_list(t_bld_in **head);
int			is_builtin(t_tree_node *node);
int			execute_builtin(t_tree_node *node, t_context *msh);
void		add_builtin(t_bld_in **head, const char *name,
				int (*func)(struct s_tree_node *node, t_context *msh));
int			ft_is_valid_env_name(const char *name);
int			is_valid_key(const char *key);

#endif
