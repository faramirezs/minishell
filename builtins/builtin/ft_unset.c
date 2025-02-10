/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mestefan <mestefan@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 01:21:48 by mestefan          #+#    #+#             */
/*   Updated: 2025/02/10 01:21:51 by mestefan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/built_in.h"
#include "../../headers/minishell.h"

int	ms_unset_export(t_context *msh, const char *key)
{
	int		i;
	size_t	key_len;

	i = 0;
	key_len = ft_strlen(key);
	while (msh->env_export[i])
	{
		if (ft_strncmp(msh->env_export[i], key, key_len) == 0
			&& msh->env_export[i][key_len] == '=')
		{
			free(msh->env_export[i]);
			while (msh->env_export[i + 1])
			{
				msh->env_export[i] = msh->env_export[i + 1];
				i++;
			}
			msh->env_export[i] = NULL;
			return (0);
		}
		i++;
	}
	return (-1);
}

int	handle_unset(t_tree_node *node, t_context *msh)
{
	char	**av;
	int		i;
	int		status;

	av = node->data.exec_u.args;
	i = 1;
	status = 0;
	if (!av[1])
	{
		status = 1;
		msh->ret_exit = status;
		return (1);
	}
	while (av[i])
	{
		if (ms_unset_env(msh, av[i]) != 0 || ms_unset_export(msh, av[i]) != 0)
			status = 1;
		i++;
	}
	msh->ret_exit = status;
	return (status);
}
