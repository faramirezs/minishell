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

//# include "../headers/tree_node.h"
//# include "tree_node.h"
//# include "../headers/minishell.h"

int		exec(t_tree_node *node, t_context *msh);
void	cleanup(t_tree_node *node, int exit_code);
int		command_uses_stdin(const char *cmd);

#endif
