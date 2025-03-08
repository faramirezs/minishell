/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 17:12:47 by alramire          #+#    #+#             */
/*   Updated: 2025/03/08 19:31:23 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "minishell.h"

int		exec(t_tree_node *node, t_context *msh);
void	cleanup(t_scanner *scanner, t_tree_node *node, int exit_code);
int		exec_node(t_tree_node *node, t_context *ctx);
int		exec_command(t_tree_node *node, t_context *ctx);
int		exec_pipe(t_tree_node *node, t_context *ctx);
int		exec_redir(t_tree_node *node, t_context *ctx);
int		handle_heredoc(t_redircmd *rcmd);
int		setup_pipe_redirection(t_context *ctx, int saved_stdin,
			int saved_stdout, t_tree_node *node);
int		save_std_fds(int *saved_stdin, int *saved_stdout, t_tree_node *node);
void	restore_std_fds(int saved_stdin, int saved_stdout, t_tree_node *node);
void	exec_child_process(t_tree_node *node, t_context *ctx);

#endif
