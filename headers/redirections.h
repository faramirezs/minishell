/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 17:40:19 by alramire          #+#    #+#             */
/*   Updated: 2025/01/17 15:51:14 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTIONS_H
# define REDIRECTIONS_H

# include "tree_node.h"


t_tree_node					*parse_redir(t_scanner *scanner, t_args *cmd_args);
int							check_redir(t_scanner *scanner);
int							determine_target_type(const char *target);
int							get_redir_flags(int redir_type);
int							handle_heredoc(t_redircmd *rcmd);
void						cleanup_heredoc(t_redircmd *rcmd);
void						handle_heredoc_sigint(int sig);
void						setup_heredoc_signals(void);
int							is_heredoc_interrupted(void);
void						reset_heredoc_interrupt(void);
char						*collect_heredoc_input(const char *delimiter);
void						restore_stdin(void);
//void						save_terminal_settings(void);
//void						restore_terminal_settings(void);

//int							handle_input_redirection(t_redircmd *rcmd);
#endif
