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

int	ms_set_env(char **env, t_context *msh, const char *value)
{
	int			i;
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
	msh->env = ms_matrix_add_line(env, value);
	if (!msh->env)
		return (-1);
	return (0);
}
//
//static int	ft_update_env_var(char **env, const char *value, size_t key_len)
//{
//	size_t	i;
//
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
//	return (-1);
//}
//
//int	ms_set_env(char **env, t_context *msh, const char *value)
//{
//	char	*new_value;
//	size_t	key_len;
//        int		i;
//
//        i = 0;
//	key_len = ft_strchr(value, '=') - value;
//	if (ft_update_env_var(env, value, key_len) == 0)
//		return (0);
//	new_value = ft_strjoin_free_s1(ft_strdup(value), "=");
//	if (!new_value)
//		return (-1);
//	env = realloc(env, sizeof(char *) * (i + 2));
//	if (!env)
//	{
//		free(new_value);
//		return (-1);
//	}
//	env[i] = new_value;
//	env[i + 1] = NULL;
//	return (0);
//}
