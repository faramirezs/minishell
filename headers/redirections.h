/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandroramirez <alejandroramirez@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 17:40:19 by alramire          #+#    #+#             */
/*   Updated: 2025/01/12 19:19:10 by alejandrora      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTIONS_H
# define REDIRECTIONS_H

# include "tree_node.h"


t_tree_node					*parse_redir(t_scanner *scanner, t_args *cmd_args);
int							check_redir(t_scanner *scanner);
int							determine_target_type(const char *target);
int							get_redir_flags(int redir_type);
int							handle_input_redirection(t_redircmd *rcmd);
#endif
