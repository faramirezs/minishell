#include "../headers/built_in.h"
#include "../headers/minishell.h"

int	handle_unset(t_tree_node *node, t_context *msh)
{
	char	**av;
	int		i;
	int		status;

	status = 0;
	av = node->data.exec_u.args;
	if (!av[1])
	{
		fprintf (stderr, "unset: missing argument\n");
		msh->ret_exit = 1;
		return (1);
	}
	i = 1;
	while (av[i])
	{
		if (handle_env(node, msh) != 0)
		{
			fprintf (stderr, "unset: %s failed to remove\n", av[i]);
			status = 1;
		}
		i++;
	}
	msh->ret_exit = status;
	return (status);
}
