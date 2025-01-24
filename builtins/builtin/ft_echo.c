#include "../headers/built_in.h"
#include "../headers/minishell.h"

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

#include "../headers/built_in.h"
#include "../headers/minishell.h"

int handle_echo(struct s_tree_node *node, t_context *msh)
{
    int i = 1;
    int n = false;
    char *expanded_arg;

    if (!node || !node->data.exec_u.args)
        return 1;

    // Handle -n option
    while (node->data.exec_u.args[i] && ft_newline(node->data.exec_u.args[i]))
    {
        n = true;
        i++;
    }

    // Print arguments with expansion
    while (node->data.exec_u.args[i])
    {
        if (node->data.exec_u.args[i][0] == '$')
        {
            // Skip the '$' character
            expanded_arg = expand_env_var_value(node->data.exec_u.args[i] + 1, msh);
        }
        else
        {
            expanded_arg = ft_strdup(node->data.exec_u.args[i]);
        }

        printf("%s", expanded_arg);
        if (node->data.exec_u.args[i + 1])
            printf(" ");
        free(expanded_arg);
        i++;
    }

    if (!n)
        printf("\n");
    return 0;
}
