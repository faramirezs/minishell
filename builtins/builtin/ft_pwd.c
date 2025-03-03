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
	char	*pwd;

	(void)msh;
	(void)node;
	pwd = ms_get_varenv(msh->env, "PWD");
	if (pwd)
	{
		printf("%s\n", pwd);
		return (0);
	}
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		perror("pwd");
		return (1);
	}
	printf("%s\n", pwd);
	free(pwd);
	return (0);
}
