/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 15:20:21 by alramire          #+#    #+#             */
/*   Updated: 2025/02/07 16:21:20 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_REDIR_H
# define EXEC_REDIR_H

# include "minishell.h"

int handle_echo_redirection(t_redircmd *rcmd, t_context *ctx);
int execute_redirection(t_redircmd *rcmd, t_context *ctx, int saved_stdin, int saved_stdout);
void cleanup_fds(int saved_stdin, int saved_stdout, t_tree_node *node, int exit_code);
int save_original_fds(int *saved_stdin, int *saved_stdout, t_tree_node *node);
int restore_fds(int saved_stdin, int saved_stdout);
int setup_pipe_redirections(t_context *ctx, int saved_stdin, int saved_stdout, t_tree_node *node);
int handle_input_redirection(t_redircmd *rcmd, int saved_stdin, int saved_stdout);
int handle_output_redirection(t_redircmd *rcmd, int saved_stdin, int saved_stdout);
int exec_redir(t_tree_node *node, t_context *ctx);

#endif
