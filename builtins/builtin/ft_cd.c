#include "../headers/built_in.h"
#include "../headers/structs.h"

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

	dir = node->data.exec_u.args[1];
	if (dir == NULL)
	{
		dir = ms_get_env (msh->env, "HOME") + 5;
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
		if (dir[0] == '\0')
			return (1);
		ft_putstr_fd ("cd: ", STDERR_FILENO);
		ft_putstr_fd (dir, STDERR_FILENO);
		ft_putendl_fd (": no such file or dir", STDERR_FILENO);
		return (1);
	}
	set_pwd (msh, "OLDPWD=", cwd);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		set_pwd (msh, "PWD=", cwd);
	else
		perror("getcwd");
	return (0);
}
