#include "../headers/built_in.h"
#include "../headers/minishell.h"

int handle_env(struct s_tree_node *node, t_context *msh)
{
	(void)node;
	
	if (!msh->env)
	{
		fprintf(stderr, "Error: msh->env is NULL in handle_env\n");
		return (1);
	}
	
	return (0);
}
