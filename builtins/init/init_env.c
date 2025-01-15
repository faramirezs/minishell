#include "../../headers/structs.h"
#include "../../headers/env_var.h"

void	check_shlvl(t_context *t_msh)
{
	char	*var;
	char	*value;
	int	 shlvl;
	char	*new_var;

	var = ms_get_env (t_msh->env, "SHLVL");
	if (!var)
	{
		t_msh->env = ms_matrix_add_line(t_msh->env, "SHLVL=1");
		return ;
	}
	value = ms_get_varenv (t_msh->env, "SHLVL");
	if (!value || ft_isdigit(value[0]) == 0)
	{
		ft_printf("Warning: invalid SHLVL value. Resetting to 1\n");
		free (value);
		t_msh->env = ms_matrix_add_line (t_msh->env, "SHLVL=1");
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
	ms_set_env (t_msh->env, new_var);
	free (new_var);
}

void	init_env(char **env, t_context *t_msh)
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
	t_msh->env = malloc(sizeof(char *) * (i + 1));
	if (!t_msh->env)
	{
		perror ("malloc failed\n");
		exit (EXIT_FAILURE);
	}
	i = 0;
	while (env[i])
	{
		t_msh->env[i] = ft_strdup (env[i]);
		i++;
	}
	t_msh->env[i] = NULL;
	check_shlvl(t_msh);
}
