#include "../../headers/minishell.h"
#include "../../headers/env_var.h"

void	check_shlvl(t_context *msh)
{
	char	*var;
	char	*value;
	int	 	shlvl;
	char	*new_var;

	var = ms_get_env (msh->env, "SHLVL");
	if (!var)
	{
		msh->env = ms_matrix_add_line(msh->env, "SHLVL=1");
		return ;
	}
	value = ms_get_varenv (msh->env, "SHLVL");
	if (!value || ft_isdigit(value[0]) == 0)
	{
		printf("Warning: invalid SHLVL value. Resetting to 1\n");
		free (value);
		msh->env = ms_matrix_add_line (msh->env, "SHLVL=1");
		return ;
	}
	shlvl = ft_atoi (value);
	free (value);
	if (shlvl < 0)
		shlvl = 1;
	else
		shlvl++;
	value = ft_itoa (shlvl);
	new_var = ft_strjoin ("SHLVL=", value);
	free (value);
	ms_set_env (msh->env, msh, new_var);
	free (new_var);
}

void	init_env(char **env, t_context *msh)
{
	int i;

	i = 0;
	if (!env)
	{
		fprintf(stderr, "Error: env is NULL\n");
		exit(EXIT_FAILURE);
	}
	while (env[i])
		i++;
	msh->env = OOM_GUARD(malloc(sizeof(char *) * (i + 1)), __FILE__, __LINE__);
	i = 0;
	while (env[i])
	{
		msh->env[i] = ft_strdup (env[i]);
		i++;
	}
	msh->env[i] = NULL;
	check_shlvl(msh);
}
