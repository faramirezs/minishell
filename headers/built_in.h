/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 11:39:52 by alramire          #+#    #+#             */
/*   Updated: 2025/02/15 11:40:08 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILT_IN_H
# define BUILT_IN_H

# include "minishell.h"

typedef struct bld_in
{
	char *name;
	int (*func)(t_tree_node *node, t_context *msh);
	struct bld_in *next;
}		bld_in;

bld_in	*find_builtin(bld_in *head, t_tree_node *node);
bld_in	*create_builtin_list(void);
int		handle_cd(struct s_tree_node *node, t_context *msh);
int		handle_echo(struct s_tree_node *node, t_context *msh);
int		handle_exit(struct s_tree_node *node, t_context *msh);
int		handle_export(struct s_tree_node *node, t_context *msh);
int		handle_pwd(struct s_tree_node *node, t_context *msh);
int		handle_unset(struct s_tree_node *node, t_context *msh);
int		handle_env(struct s_tree_node *node, t_context *msh);
void	free_builtin_list(bld_in *head);
int		is_builtin(t_tree_node *node);
int		execute_builtin(t_tree_node *node, t_context *msh);
void	add_builtin(bld_in **head, const char *name,
			int (*func)(struct s_tree_node *node, t_context *msh));
int		ft_is_valid_env_name(const char *name);

#endif
