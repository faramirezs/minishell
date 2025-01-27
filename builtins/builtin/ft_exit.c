#include "../headers/built_in.h"
#include "../headers/minishell.h"

int handle_exit(struct s_tree_node *node, t_context *msh)
{
	long	exit_code;
	const char	*av;
	char	*endptr;

	if (!node || !node->data.exec_u.args[1] || !node->data.exec_u.args)
	{
		msh->ret_exit = 0;
		exit(0);
	}
	av = node->data.exec_u.args[1];
	endptr = NULL;
	exit_code = ft_strtol (av, &endptr, 10);
	if (*endptr != '\0' || exit_code < 0 || exit_code > 255)
	{
		fprintf (stderr, "exit: %s numeric av requided\n", av);
		msh->ret_exit = 255;
		exit (255);
	}
	msh->ret_exit = (int)exit_code;
	//free_string_array(&node->data.exec_u.args);
	cleanup(node, msh->ret_exit);
	return (int)exit_code;
	//exit ((int)exit_code);
}
