/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mestefan <mestefan@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 01:06:24 by mestefan          #+#    #+#             */
/*   Updated: 2025/02/16 10:45:16 by mestefan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/minishell.h"
#include "headers/env_var.h"

//int	ms_set_env(char **env, t_context *msh, const char *value)
//{
//	int			i;
//	const char	*equals;
//	size_t		key_len;
//
//	if (!env)
//		return (-1);
//	equals = ft_strchr(value, '=');
//	if (!equals)
//	{
//		fprintf(stderr, "setenv: %s: Invalid argument\n", value);
//		return (-1);
//	}
//	key_len = equals - value;
//	i = 0;
//	while (env[i])
//	{
//		if (ft_strncmp(env[i], value, key_len) == 0 && env[i][key_len] == '=')
//		{
//			free(env[i]);
//			env[i] = ft_strdup(value);
//			if (!env[i])
//				return (-1);
//			return (0);
//		}
//		i++;
//	}
//	msh->env = ms_matrix_add_line(env, value);
//	if (!msh->env)
//		return (-1);
//	return (0);
//}
static int	ms_update_env_entry(char **env, const char *value, size_t key_len)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], value, key_len) == 0 && env[i][key_len] == '=')
		{
			free(env[i]);
			env[i] = ft_strdup(value);
			if (!env[i])
				return (-1);
			return (0);
		}
		i++;
	}
	return (-1);
}

int	ms_set_env(char **env, t_context *msh, const char *value)
{
	const char	*equals;
	size_t		key_len;

	if (!env)
		return (-1);
	equals = ft_strchr(value, '=');
	if (!equals)
	{
		fprintf(stderr, "setenv: %s: Invalid argument\n", value);
		return (-1);
	}
	key_len = equals - value;
	if (ms_update_env_entry(env, value, key_len) == 0)
		return (0);
	msh->env = ms_matrix_add_line(env, value);
	if (msh->env != NULL)
    	return (0);
	else
    	return (-1);
}
