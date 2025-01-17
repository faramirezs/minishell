#include "../../headers/built_in.h"
#include "../../headers/minishell.h"

void	set_pwd(t_context *msh, char *av, char *c)
{
	char	*string;

	string = ft_strjoin(av, c);
	ms_set_env (msh->env, string);
	free (string);
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
		if ((dir - 5) == NULL || *dir == '\0')
		{
			ft_putstr_fd ("cd: HOME not set\n", STDERR_FILENO);
			return (1);
		}
	}
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("getcwd");
		return (1);
	}
	if (chdir(dir) == -1)
	{
		fprintf (stderr, "cd: %s no such file or dir", dir);
		return (1);
	}
	set_pwd (msh, "OLDPWD=", cwd);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		set_pwd (msh, "PWD=", cwd);
	else
		perror("getcwd");
	return (0);
}
