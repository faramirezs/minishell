#include "../headers/built_in.h"
#include "../headers/structs.h"

int handle_env(struct s_tree_node *node, t_context *msh)
{
	(void)node;
	int i;

	i = 0;
	if (!msh->env)
    	return 1;
	while (msh->env[i])
	{
		printf("%s\n", msh->env[i++]);
	}
	return (0);
}
