/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 01:11:34 by mestefan          #+#    #+#             */
/*   Updated: 2025/03/08 13:27:59 by alramire         ###   ########.fr       */
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
		if (ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "echo") == 0
			|| ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "export") == 0
			|| ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0
			|| ft_strcmp(cmd, "exit") == 0)
			return (1);
	}
	return (0);
}

int	execute_builtin(t_tree_node *node, t_context *msh)
{
	t_bld_in	*builtin;

	builtin = find_builtin(msh->builtins, node);
	if (builtin)
		builtin->func(node, msh);
	else
	{
		fprintf(stderr, "minishell: %s: command not found\n",
			node->data.exec_u.args[0]);
		msh->ret_exit = 127;
	}
	return (msh->ret_exit);
}
