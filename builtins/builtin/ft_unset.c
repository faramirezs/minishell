#include "../headers/built_in.h"
#include "../headers/minishell.h"

int ms_unset_export(t_context *msh, const char *key)
{
    int i = 0;
    size_t key_len = ft_strlen(key);

    while (msh->env_export[i])
    {
        if (ft_strncmp(msh->env_export[i], key, key_len) == 0 && msh->env_export[i][key_len] == '=')
        {
            free(msh->env_export[i]);
            while (msh->env_export[i + 1])
            {
                msh->env_export[i] = msh->env_export[i + 1];
                i++;
            }
            msh->env_export[i] = NULL;
            return 0;
        }
        i++;
    }

    return -1;
}


int handle_unset(t_tree_node *node, t_context *msh)
{
    char	**av;
    int		i;
    int		status;

	av = node->data.exec_u.args;
	i = 1;
	status = 0;
    if (!av[1])
    {
        //fprintf(stderr, "unset: missing argument\n");
        msh->ret_exit = 1;
        return (1);
    }

    while (av[i])
    {
        if (ms_unset_env(msh, av[i]) != 0 || ms_unset_export(msh, av[i]) != 0)
        {
            //fprintf(stderr, "unset: %s failed to remove\n", av[i]);
            status = 1;
        }
        i++;
    }
    
    msh->ret_exit = status;
    return (status);
}
