#include "../headers/built_in.h"
#include "../headers/structs.h"

int	ft_newline(char *av)
{
	int	i;

	i = 0;
	if (av && av[i++] == '-')
	{
		while (av[i])
		{
			if (av[i] != 'n')
				break ;
			if (av[i + 1] == '\0')
				return (true);
			i++;
		}
	}	
	return (false);
}

int	handle_echo(struct s_tree_node *node, t_context *msh)
{
	int i;
	int n;
	(void)msh;

	if (!node || node->type != N_EXEC || !node->data.exec_u.args)
        return (1);
	i = 1;
	n = false;
	while (node->data.exec_u.args[i] && ft_newline(node->data.exec_u.args[i]))
	{
		n = true;
		i++;
	}
	while (node->data.exec_u.args[i])
	{
		printf ("%s", node->data.exec_u.args[i]);
		if (node->data.exec_u.args[i + 1])
			printf(" ");
		i++;
	}
	if (n == false)
		printf ("\n");
	return (0);
}
