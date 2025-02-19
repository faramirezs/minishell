/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mestefan <mestefan@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 01:02:51 by mestefan          #+#    #+#             */
/*   Updated: 2025/02/16 01:04:37 by mestefan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/minishell.h"
#include "headers/env_var.h"

int	ms_unset_env(t_context *msh, const char *key)
{
	int		i;
	size_t	key_len;

	i = 0;
	key_len = ft_strlen(key);
	while (msh->env[i])
	{
		if (ft_strncmp(msh->env[i], key, key_len) == 0
			&& msh->env[i][key_len] == '=')
		{
			free(msh->env[i]);
			while (msh->env[i + 1])
			{
				msh->env[i] = msh->env[i + 1];
				i++;
			}
			msh->env[i] = NULL;
			return (0);
		}
		i++;
	}
	return (-1);
}

void	free_env(char **env)
{
	int	i;

	if (env)
	{
		i = 0;
		while (env[i])
		{
			free (env[i]);
			env[i] = NULL;
			i++;
		}
	}
	free(env);
	env = NULL;
}

void	cleanup_context(t_context *msh)
{
	if (msh)
	{
		if (msh->env)
		{
			free_env(msh->env);
			msh->env = NULL;
		}
		if (msh->env_export)
		{
			free_env(msh->env_export);
			msh->env_export = NULL;
		}
		free(msh);
		msh = NULL;
	}
	ft_free_tab(msh->env); //TODO SEG FAULT when nothing ever was entered into any line
}

char	**duplicate_env(char **env)
{
	int		i;
	char	**copy;

	i = 0;
	while (env[i])
		i++;
	copy = oom_guard(malloc(sizeof(char *) * (i + 1)), __FILE__, __LINE__);
	i = 0;
	while (env[i])
	{
		copy[i] = ft_strdup(env[i]);
		if (!copy[i])
		{
			ft_free_tab(copy);
			return (NULL);
		}
		i++;
	}
	copy[i] = NULL;
	return (copy);
}
