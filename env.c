/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mestefan <mestefan@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 20:46:07 by mestefan          #+#    #+#             */
/*   Updated: 2025/02/15 20:46:15 by mestefan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/minishell.h"
#include "headers/env_var.h"

t_context	*init_context(char **envp)
{
	t_context	*msh;

	msh = oom_guard(malloc(sizeof(t_context)), __FILE__, __LINE__);
	msh->fd[0] = STDIN_FILENO;
	msh->fd[1] = STDOUT_FILENO;
	msh->fd_close = -1;
	msh->ret_exit = 0;
	msh->user = getenv("USER");
	if (envp)
	{
		msh->env = duplicate_env(envp);
		msh->env_export = duplicate_env(envp);
	}
	else
	{
		msh->env = NULL;
		msh->env_export = NULL;
	}
	check_shlvl(msh);
	return (msh);
}

char	*ms_get_env(char **env, const char *key)
{
	int		i;
	size_t	key_len;
	char	*equals;

	i = 0;
	key_len = ft_strlen(key);
	while (env[i])
	{
		equals = ft_strchr(env[i], '=');
		if (equals)
		{
			if (ft_strncmp(env[i], key, key_len) == 0)
				return (equals + 1);
		}
		i++;
	}
	return (NULL);
}

char	*ms_get_varenv(char **env, char *av)
{
	int		i;
	size_t	key_len;
	char	*equals;

	i = 0;
	key_len = ft_strlen(av);
	while (env[i])
	{
		equals = ft_strchr(env[i], '=');
		if (equals && (size_t)(equals - env[i]) == key_len
			&& ft_strncmp(env[i], av, key_len) == 0)
			return (equals + 1);
		i++;
	}
	return (NULL);
}

int	env_compare(char **env, char **av)
{
	return (find_env_index(env, av[0]));
}
