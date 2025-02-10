/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mestefan <mestefan@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 01:19:27 by mestefan          #+#    #+#             */
/*   Updated: 2025/02/10 01:19:31 by mestefan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/built_in.h"
#include "../../headers/minishell.h"

int	handle_env(struct s_tree_node *node, t_context *msh)
{
	int	i;

	(void)node;
	i = 0;
	if (!msh->env)
	{
		fprintf(stderr, "Error: msh->env is NULL in handle_env\n");
		msh->ret_exit = 1;
		return (1);
	}
	while (msh->env[i])
	{
		printf("%s\n", msh->env[i++]);
	}
	msh->ret_exit = 0;
	return (0);
}
