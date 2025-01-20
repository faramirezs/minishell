#include "../headers/built_in.h"
#include "../headers/minishell.h"

int handle_env(struct s_tree_node *node, t_context *msh)
{
	(void)node;
	int i;

	printf("handle_env: msh->env = %p\n", (void *)msh->env);
	for (int i = 0; msh->env && msh->env[i]; i++)
    	printf("  msh->env[%d]: %s\n", i, msh->env[i]);

	i = 0;
	if (!msh->env)
	{
		fprintf(stderr, "Error: msh->env is NULL in handle_env\n");
    	return (1);
	}
	while (msh->env[i])
	{
		printf("%s\n", msh->env[i++]);
	}
	return (0);
}
