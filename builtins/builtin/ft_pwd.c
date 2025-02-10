/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mestefan <mestefan@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 01:21:06 by mestefan          #+#    #+#             */
/*   Updated: 2025/02/10 01:21:38 by mestefan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/built_in.h"
#include "../../headers/minishell.h"

int	handle_pwd(struct s_tree_node *node, t_context *msh)
{
	char	cwd[1024];

	(void)msh;
	if (!node || !node->data.exec_u.args)
		return (1);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		return (0);
	}
	else
	{
		perror ("pwd");
		return (1);
	}
}
