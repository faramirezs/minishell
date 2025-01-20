#include "../headers/built_in.h"
#include "../headers/minishell.h"

int handle_exit(struct s_tree_node *node, t_context *msh)
{
	long long   exit_code;
	int		 i;

	i = 0;
	if (!node || !node->data.exec_u.args[1] || !node->data.exec_u.args)
		return (0);
	while (node->data.exec_u.args[1][i] == ' ')
		i++;
	if (node->data.exec_u.args[1][i] == '+' || node->data.exec_u.args[1][i] == '-')
		i++;
	if (!ft_isdigit(node->data.exec_u.args[1][i]))
	{
		printf ("exit: %s isn't numeric argument\n", node->data.exec_u.args[1]);
		msh->ret_exit = 255;
		return (1);
	}
	exit_code = ft_atoll(node->data.exec_u.args[1]);
	if (ft_isllong(node->data.exec_u.args[1]) != 0 || exit_code < LLONG_MIN || exit_code > LLONG_MAX)
	{
		printf ("exit: %s isn't numeric", node->data.exec_u.args[1]);
		msh->ret_exit = 255;
		return (1);
	}
	msh->ret_exit = exit_code % 256;
	if (msh->ret_exit < 0)
		msh->ret_exit += 256;
	return (0);
}
