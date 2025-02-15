/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 17:12:47 by alramire          #+#    #+#             */
/*   Updated: 2025/02/15 17:15:34 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "minishell.h"

int		exec(t_tree_node *node, t_context *msh);
void	cleanup(t_tree_node *node, int exit_code);
int		exec_node(t_tree_node *node, t_context *ctx);
int		exec_command(t_tree_node *node, t_context *ctx);
int		exec_pipe(t_tree_node *node, t_context *ctx);
int		exec_redir(t_tree_node *node, t_context *ctx);
int		handle_heredoc(t_redircmd *rcmd);

#endif
