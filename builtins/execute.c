/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mestefan <mestefan@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 01:11:34 by mestefan          #+#    #+#             */
/*   Updated: 2025/02/10 01:11:40 by mestefan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "../headers/built_in.h"

int	is_builtin(t_tree_node *node)
{
	const char	*cmd;

	if (node->type == N_EXEC && node->data.exec_u.args[0] != NULL)
	{
		cmd = node->data.exec_u.args[0];
		if (strcmp(cmd, "cd") == 0 || strcmp(cmd, "echo") == 0
			|| strcmp(cmd, "pwd") == 0 || strcmp(cmd, "export") == 0
			|| strcmp(cmd, "unset") == 0 || strcmp(cmd, "env") == 0
			|| strcmp(cmd, "exit") == 0)
			return (1);
	}
	return (0);
}

int	execute_builtin(t_tree_node *node, t_context *msh)
{
	bld_in	*builtins;
	bld_in	*builtin;

	builtins = create_builtin_list();
	builtin = find_builtin(builtins, node);
	if (builtin)
		builtin->func(node, msh);
	else
	{
		fprintf(stderr, "minishell: %s: command not found\n",
			node->data.exec_u.args[0]);
		msh->ret_exit = 127;
	}
	free_builtin_list(builtins);
	return (msh->ret_exit);
}
