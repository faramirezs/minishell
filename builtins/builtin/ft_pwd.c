#include "../headers/built_in.h"
#include "../headers/structs.h"

int	handle_pwd(struct s_tree_node *node, t_context *msh)
{
	(void)msh;
	char	cwd[1024];

	if (!node || !node->data.exec_u.args)
        return (1);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		return (0);
	}
	else
	{
		perror ("pwd");
		return (1);
	}
}