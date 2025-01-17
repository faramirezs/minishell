#include "../headers/minishell.h"
#include "../headers/structs.h"
#include "../headers/env_var.h"

char	*ms_get_env(char **env, const char *key)
{
	int	 i;
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
	int	 i;
	size_t	key_len;
	char	*equals;

	i = 0;
	key_len = ft_strlen(av);
	while (env[i])
	{
		equals = ft_strchr(env[i], '=');
		if (equals && (size_t)(equals - env[i]) == key_len && ft_strncmp(env[i], av, key_len))
			return (equals + 1);
		i++;
	}
	return (NULL);
}

int env_compare(char **env, char **av)
{
	return (find_env_index(env, av[0]));
}

int	ms_set_env(char **env, t_context *msh, const char *value)
{
	int	 i;
	const char	*equals;
	size_t	key_len;

	equals = ft_strchr(value, '=');
	if (!equals)
	{
		ft_printf("setenv: %s: Invalid argument\n", value);
		return -1;
	}
	i = 0;
	key_len = equals - value;
	while ((*env)[i])
	{
		if (ft_strncmp((*env)[i], value, key_len) == 0)
		{
			free((*env)[i]);
			(*env)[i] = ft_strdup(value);
			if (!(*env)[i])
				return -1;
			return 0;
		}
		i++;
	}
	msh->env = ms_matrix_add_line(msh->env, value);
	if (!msh->env)
		return -1;
	*env = msh->env;
	return (0);
}

static int find_env_index(char **env, const char *key)
{
	int	 i;
	size_t	key_len;
	char	*equals;

	i = 0;
	key_len = ft_strlen(key);
	while (env[i])
	{
		equals = ft_strchr(env[i], '=');
		if (equals && (size_t)(equals - env[i]) == key_len && ft_strncmp(env[i], key, key_len) == 0)
			return (i);
		i++;
	}
	return -1;
}

char **ms_remove_line(char **matrix, int index)
{
	int	 i;
	int	 j;
	int	 size;
	char	**new_matrix;

	i = 0;
	j = 0;
	size = 0;
	while (matrix[size])
		size++;
	new_matrix = malloc(sizeof(char *) * size);
	if (!new_matrix)
		return (NULL);
	while (matrix[i])
	{
		if (i != index)
		{
			new_matrix[j] = ft_strdup(matrix[i]);
			if (!new_matrix[j])
			{
				ft_free_tab (new_matrix);
				return (NULL);
			}
			j++;
		}
		i++;
	}
	new_matrix[j] = NULL;
	ft_free_tab(matrix);
	return new_matrix;
}

int ms_unset_env(t_context *msh, const char *key)
{
	int index;

	index = find_env_index(msh->env, key);
	if (index != -1)
	{
		msh->env = ms_remove_line(msh->env, index);
		if (!msh->env)
			return (1);
		return (0);
	}
	return(1);
}

void free_env(char **env)
{
    for (int i = 0; env[i]; i++)
        free(env[i]);
    free(env);
}


