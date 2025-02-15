* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mestefan <mestefan@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 01:19:49 by mestefan          #+#    #+#             */
/*   Updated: 2025/02/10 01:19:53 by mestefan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../headers/built_in.h"
#include "../../headers/minishell.h"

void	set_pwd(t_context *msh, char *av, char *cwd)
{
	char	*string;

	string = ft_strjoin(av, cwd);
	if (!string)
		return ;
	if (ms_set_env(msh->env, msh, string) == -1)
		ft_putstr_fd("Failed to set environment variable\n", STDERR_FILENO);
	free(string);
}

int	handle_cd(struct s_tree_node *node, t_context *msh)
{
	char	cwd[1024];
	char	*dir;

	if (!node || !node->data.exec_u.args)
		return (1);
	dir = node->data.exec_u.args[1];
	if (dir == NULL)
	{
		dir = ms_get_env (msh->env, "HOME");
		if (!dir || *dir == '\0')
		{
			ft_putstr_fd ("cd: HOME not set\n", STDERR_FILENO);
			return (1);
		}
	}
//	else if (node->data.exec_u.args[2])
//	{
//		fprintf(stderr, "minishell: cd: too many arguments\n");
//		msh->ret_exit = 1;
//		return (1);
//	}
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("getcwd");
		msh->ret_exit = 1;
		return (1);
	}
	if (chdir(dir) == -1)
	{
		fprintf (stderr, "cd: %s no such file or dir\n", dir);
		msh->ret_exit = 1;
		return (1);
	}
	set_pwd (msh, "OLDPWD=", cwd);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		set_pwd (msh, "PWD=", cwd);
	else
		perror("getcwd");
	msh->ret_exit = 0;
	return (0);
}
