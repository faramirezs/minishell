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
	int		index;
	size_t	key_len;

	i = 0;
	key_len = ft_strlen(key);
	index = find_env_index(msh->env_export, key);
	if (index == -1)
		return (-1);
	free(msh->env_export[index]);
	i = index;
	while (msh->env_export[i + 1])
	{
		msh->env_export[i] = msh->env_export[i + 1];
		i++;
	}
	return (-1);
}

int	unset_variable(t_context *msh, const char *var)
{
	if (!is_valid_key(var))
	{
		fprintf(stderr, "unset: `%s`: invalid parameter name\n", var);
		return (1);
	}
	ms_unset_env(msh, var);
	ms_unset_export(msh, var);
	return (0);
}

int	handle_unset(t_tree_node *node, t_context *msh)
{
	char	**av;
	int		i;
	int		status;

	av = node->data.exec_u.args;
	if (!av[1])
		return (fprintf(stderr,
			"unset: not enough arguments\n"), msh->ret_exit = 1);
	i = 1;
	status = 0;
	while (av[i])
		status |= unset_variable(msh, av[i++]);
	return (msh->ret_exit = status);
}
//int	handle_unset(t_tree_node *node, t_context *msh)
//{
//	char	**av;
//	int		i;
//
//	av = node->data.exec_u.args;
//	i = 1;
//	if (!av[1])
//	{
//		fprintf(stderr, "unset: not enough arguments\n");
//		msh->ret_exit = 1;
//		return (1);
//	}
//	while (av[i])
//	{
//		if (!is_valid_key(av[i]))
//		{
//			fprintf(stderr, "unset: `%s`: invalid parameter name\n", av[i]);
//			msh->ret_exit = 1;
//		}
//		else
//		{
//			ms_unset_env(msh, av[i]);
//			ms_unset_export(msh, av[i]);
//		}
//		i++;
//	}
//	msh->ret_exit = 0;
//	return (msh->ret_exit);
//}
