#include "../headers/minishell.h"
#include "../headers/env_var.h"

t_context *init_context(char **envp)
{
	t_context *msh;
	int i;

	msh = OOM_GUARD(malloc(sizeof(t_context)), __FILE__, __LINE__);
	msh->fd[0] = STDIN_FILENO;
	msh->fd[1] = STDOUT_FILENO;
	msh->fd_close = -1;
	msh->ret_exit = 0;
	msh->user = getenv("USER");
	msh->env = NULL;
	if (envp && envp[0])
	{
		i = 0;
		while (envp[i])
		{
			msh->env = ms_matrix_add_line(msh->env, envp[i]);
			i++;
		}
	}
	else
		fprintf(stderr, "Warning: envp is NULL or empty\n");
	msh->env_export = NULL;
	if (envp && envp[0])
	{
		i = 0;
		while (envp[i])
		{
			msh->env_export = ms_matrix_add_line(msh->env_export, envp[i]);
			i++;
		}
	}
	else
		fprintf(stderr, "Warning: envp is NULL or empty\n");
	return msh;
}


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
/*
int	ms_set_env(char **env, t_context *msh, const char *value)
{
	int	 i;
	const char	*equals;
	size_t	key_len;

	equals = ft_strchr(value, '=');
	if (!equals)
	{
		printf("setenv: %s: Invalid argument\n", value);
		return -1;
	}
	i = 0;
	key_len = equals - value;
	while (env[i])
	{
		if (ft_strncmp(env[i], value, key_len) == 0)
		{
			free(env[i]);
			env[i] = ft_strdup(value);
			if (!(*env)[i])
				return -1;
			return 0;
		}
		i++;
	}
	msh->env = ms_matrix_add_line(msh->env, value);
	if (!msh->env)
		return -1;
	env = msh->env;
	return (0);
}
*/

int ms_set_env(char **env, t_context *msh, const char *value)
{
	int i;
	const char *equals;
	size_t key_len;

	if (!env)
		return (-1);
	equals = ft_strchr(value, '=');
	if (!equals)
	{
		fprintf(stderr, "setenv: %s: Invalid argument\n", value);
		return -1;
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
				return -1;
			return 0;
		}
		i++;
	}

	msh->env = ms_matrix_add_line(env, value);
	if (!msh->env)
		return -1;

	return 0;
}


int find_env_index(char **env, const char *key)
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
	int i = 0;
	size_t key_len = ft_strlen(key);

	// Search for the key in the environment
	while (msh->env[i])
	{
		if (ft_strncmp(msh->env[i], key, key_len) == 0 && msh->env[i][key_len] == '=')
		{
			// Free the matched entry and remove it from the environment
			free(msh->env[i]);
			while (msh->env[i + 1])
			{
				msh->env[i] = msh->env[i + 1];
				i++;
			}
			msh->env[i] = NULL; // Null-terminate the array
			return 0; // Success
		}
		i++;
	}

	// Key not found
	return -1;
}


void free_env(char **env)
{
	for (int i = 0; env[i]; i++)
		free(env[i]);
	free(env);
}

void cleanup_context(t_context *msh)
{
	if (msh)
	{
		free_env(msh->env);
		free_env(msh->env_export);
		free(msh);
	}
}

char *expand_env_var_value(const char *var_name, t_context *msh)
{
    if (!var_name || !msh || !msh->env)
        return ft_strdup("");
	// Handle special cases
    if (ft_strcmp(var_name, "?") == 0)
        return ft_itoa(msh->ret_exit);

    // Look up regular environment variables
    char *value = ms_get_env(msh->env, var_name);
    if (value)
        return ft_strdup(value);

    // Variable not found
    return ft_strdup("");
}
