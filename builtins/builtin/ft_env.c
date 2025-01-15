#include "../headers/built_in.h"
#include "../headers/structs.h"

int handle_env(struct tree_node *node, t_msh *t_msh)
{
	(void)node;
	int i;

	i = 0;
	if (!t_msh->env)
    	return 1;
	while (t_msh->env[i])
	{
		printf("%s\n", t_msh->env[i++]);
	}
	return (0);
}
