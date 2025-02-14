/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   exec.h											 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: alejandroramirez <alejandroramirez@stud	+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2025/01/10 17:09:13 by alramire		  #+#	#+#			 */
/*   Updated: 2025/01/15 14:04:26 by alejandrora	  ###   ########.fr	   */
/*																			*/
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
