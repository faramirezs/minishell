#include "../headers/built_in.h"
#include "../headers/minishell.h"

int process_key_value(char *arg, t_context *msh)
{
	char *equal_sign;
	char *key;
	char *value;
	char *new_var;

	equal_sign = ft_strchr(arg, '=');

	if (equal_sign)
	{
		// Temporarily split the string into key and value
		*equal_sign = '\0';
		key = arg;
		value = equal_sign + 1;

		// Add/Update the variable in the environment
		new_var = ft_strjoin(key, "=");
		new_var = ft_strjoin_free_s1(new_var, value);
		ms_set_env(msh->env, msh, new_var);

		free(new_var);
		*equal_sign = '='; // Restore the original argument
		return 1; // Successfully processed
	}
	return 0; // Failed to process (no '=' found)
}


int	handle_export(struct s_tree_node *node, t_context *msh)
{
	int		i;
	char	*arg;
	int		in_pipe;
	
	in_pipe = (msh->fd[0] != STDIN_FILENO || msh->fd[1] != STDOUT_FILENO);

	if (!node->data.exec_u.args[1])
	{
		i = 0;
		while (msh->env[i])
			printf("declare -x %s\n", msh->env[i++]);
		return (0);
	}
	i = 1;
	while (node->data.exec_u.args[i])
	{
		arg = node->data.exec_u.args[i];
		if (!process_key_value(arg, msh))
		{
			printf("export: '%s': not a valid identifier\n", arg);
			return (1);
		}
		i++;
	}
	if (in_pipe)
    {
        i = 0;
        while (msh->env[i])
        {
            dprintf(msh->fd[1], "%s\n", msh->env[i]);
            i++;
        }
    }
	return (0);
}
