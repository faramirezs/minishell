#include "../headers/built_in.h"
#include "../headers/structs.h"

int check_exit(struct s_tree_node *node, t_msh *t_msh)
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
		ft_printf ("exit: %s isn't numeric argument\n", node->data.exec_u.args[1]);
		t_msh->ret_exit = 255;
		return (1);
	}
	exit_code = ft_atoll(node->data.exec_u.args[1]);
	if (ft_isllong(node->data.exec_u.args[1]) != 0 || exit_code < LLONG_MIN || exit_code > LLONG_MAX)
	{
		ft_printf ("exit: %s isn't numeric", node->data.exec_u.args[1]);
		t_msh->ret_exit = 255;
		return (1);
	}
	t_msh->ret_exit = exit_code % 256;
	if (t_msh->ret_exit < 0)
		t_msh->ret_exit += 256;
	return (0);
}
