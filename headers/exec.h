/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   exec.h											 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: alejandroramirez <alejandroramirez@stud	+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2025/01/10 17:09:13 by alramire		  #+#	#+#			 */
/*   Updated: 2025/02/07 15:00:00 by alramire         ###   ########.fr       */
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
int exec_command(t_tree_node *node, t_context *ctx);
int exec_pipe(t_tree_node *node, t_context *ctx);
int exec_node(t_tree_node *node, t_context *ctx);


#endif
