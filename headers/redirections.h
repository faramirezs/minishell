/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 17:40:19 by alramire          #+#    #+#             */
/*   Updated: 2025/02/14 18:20:09 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTIONS_H
# define REDIRECTIONS_H

//# include "tree_node.h"
# include "minishell.h"

typedef struct s_list
{
	char			*line;
	struct s_list	*next;
}					t_list;

t_tree_node			*parse_redir(t_scanner *scanner, t_args *cmd_args,
						t_tree_node *first_redir);
int					check_redir(t_scanner *scanner);
int					determine_target_type(const char *target);
int					get_redir_flags(int redir_type);
int					handle_heredoc(t_redircmd *redir_node);
void				handle_redir_heredoc(t_redircmd *redir_node,
						t_scanner *scanner);
void				cleanup_heredoc(t_redircmd *rcmd);
void				handle_heredoc_sigint(int sig);
void				setup_heredoc_signals(void);
int					is_heredoc_interrupted(void);
void				reset_heredoc_interrupt(void);
char				*collect_heredoc_input(const char *delimiter,
						t_context *msh);
void				restore_stdin(void);
void				append_node(t_list **head, const char *line);
char				*concatenate_lines(t_list *head);
void				free_list(t_list *head);
char				*expand_env_vars(const char *line, t_context *msh);
#endif
