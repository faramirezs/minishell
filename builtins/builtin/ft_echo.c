//#include "../headers/built_in.h"
#include "../../headers/minishell.h"

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


int ft_is_valid_env_name(const char *name)
{
    int i;

    i = 0;
    if (!name || !name[0])
        return 0;
    if (!ft_isalpha(name[0]) && name[0] != '_')
        return 0;
    i = 1;
    while (name[i])
    {
        if (!ft_isalnum(name[i]) && name[i] != '_')
            return 0;
        i++;
    }
    return (1);
}

int handle_echo(struct s_tree_node *node, t_context *msh)
{
    int     i;
    int     n;
    char    *expanded_arg;
    (void)msh;

	signal(SIGPIPE, SIG_IGN);

    i = 1;
    n = false;
    if (!node || !node->data.exec_u.args)
        return 1;

    while (node->data.exec_u.args[i] && ft_newline(node->data.exec_u.args[i]))
    {
        n = true;
        i++;
    }

    while (node->data.exec_u.args[i])
    {
        expanded_arg = ft_strdup(node->data.exec_u.args[i]);
        if (write(STDOUT_FILENO, expanded_arg, ft_strlen(expanded_arg)) < 0)
        {
            free(expanded_arg);
            return 1;  // Return error if write fails
        }
        if (node->data.exec_u.args[i + 1])
        {
            if (write(STDOUT_FILENO, " ", 1) < 0)
            {
                free(expanded_arg);
                return 1;
            }
        }
        free(expanded_arg);
        i++;
    }

    if (!n && write(STDOUT_FILENO, "\n", 1) < 0)
        return 1;

    return 0;
}
