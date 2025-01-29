#include "../headers/built_in.h"
#include "../headers/minishell.h"

int handle_env(struct s_tree_node *node, t_context *msh)
{
	(void)node;
	int	i;

	i = 0;
	if (!msh->env || !msh)
	{
		fprintf(stderr, "Error: env is NULL in handle_env\n");
		msh->ret_exit = 1;
		return (1);
	}
	while (msh->env[i])
	{
		printf("%s\n", msh->env[i++]);
	}
	msh->ret_exit = 0;
	return (0);
}
