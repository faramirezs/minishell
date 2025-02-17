/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mestefan <mestefan@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 01:35:40 by mestefan          #+#    #+#             */
/*   Updated: 2025/02/10 01:35:43 by mestefan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/env_var.h"

void	handle_invalid_shlvl(t_context *msh)
{
	printf("Warning: invalid SHLVL value. Resetting to 1\n");
	msh->env = ms_matrix_add_line(msh->env, "SHLVL=1");
}

void	update_shlvl(t_context *msh, char *value)
{
	int		shlvl;
	char	*new_var;

	shlvl = ft_atoi(value);
	if (shlvl < 0)
		shlvl = 1;
	else
		shlvl++;
	value = ft_itoa(shlvl);
	new_var = ft_strjoin("SHLVL=", value);
	free(value);
	ms_set_env(msh->env, msh, new_var);
	free(new_var);
}

void	check_shlvl(t_context *msh)
{
	char	*var;
	char	*value;

	var = ms_get_env(msh->env, "SHLVL");
	if (!var)
	{
		msh->env = ms_matrix_add_line(msh->env, "SHLVL=1");
		return ;
	}
	value = ms_get_varenv(msh->env, "SHLVL");
	if (!value || ft_isdigit(value[0]) == 0)
	{
		handle_invalid_shlvl(msh);
		return ;
	}
	update_shlvl(msh, value);
}
// void	check_shlvl(t_context *msh)
// {
// 	char	*var;
// 	char	*value;
// 	int		shlvl;
// 	char	*new_var;

// 	var = ms_get_env (msh->env, "SHLVL");
// 	if (!var)
// 	{
// 		msh->env = ms_matrix_add_line(msh->env, "SHLVL=1");
// 		return ;
// 	}
// 	value = ms_get_varenv (msh->env, "SHLVL");
// 	if (!value || ft_isdigit(value[0]) == 0)
// 	{
// 		printf("Warning: invalid SHLVL value. Resetting to 1\n");
// 		msh->env = ms_matrix_add_line(msh->env, "SHLVL=1");
// 		return ;
// 	}
// 	shlvl = ft_atoi (value);
// 	if (shlvl < 0)
// 		shlvl = 1;
// 	else
// 		shlvl++;
// 	value = ft_itoa (shlvl);
// 	new_var = ft_strjoin ("SHLVL=", value);
// 	free (value);
// 	ms_set_env (msh->env, msh, new_var);
// 	free (new_var);
// }
