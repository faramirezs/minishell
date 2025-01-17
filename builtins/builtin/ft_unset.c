#include "../headers/built_in.h"
#include "../headers/structs.h"
/*
int	handle_unset(struct s_tree_node *node, t_context *msh)
{
	char	**av;

	av = node->data.exec_u.args;
	if (!av[1])
	{
		fprintf (stderr, "unset: missing argument\n");
		return (EXIT_FAILURE);
	}
	if ((ms_unset_env(t_msh->env, av[1])) != 0)
	{
		perror ("unset");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
*/