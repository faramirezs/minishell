#include "../headers/built_in.h"
#include "../headers/minishell.h"

int is_valid_key(const char *key)
{
	int i;

	i = 0;
    if (!key || (!ft_isalpha(key[0]) && key[0] != '_'))  // Key must start with a letter or underscore
        return (0);
    
    while (key[i] != '\0')  // After the first character, key must contain only alphanumeric or underscores
    {
        if (!ft_isalnum(key[i]) && key[i] != '_')
            return (0);
		i++;
    }
    return (1);
}

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

        // Check if the key is valid
        if (!is_valid_key(key))
        {
            *equal_sign = '=';
            return (0);
        }
        new_var = ft_strjoin(key, "=");
        new_var = ft_strjoin_free_s1(new_var, value);
        ms_set_env(msh->env, msh, new_var);

        free(new_var);
        *equal_sign = '='; // Restore the original argument
        return (1); // Successfully processed
    }
    else
    {
        if (is_valid_key(arg)){
            fprintf(stderr, "DEBUG: Valid key without '=': %s\n", arg);
            return (1);
        }
        fprintf(stderr, "DEBUG: Invalid key without '=': %s\n", arg);
        return (0);
    }
    return (0); // Failed to process (no '=' found)
}

int	handle_export(struct s_tree_node *node, t_context *msh)
{
	int		i;
	char	*arg;
	int		in_pipe;
    int     status;
	
	in_pipe = (msh->fd[0] != STDIN_FILENO || msh->fd[1] != STDOUT_FILENO);
    status = 0;
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
			fprintf(stderr, "export: `%s`: not a valid identifier\n", arg);
			status = 1;
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
    msh->ret_exit = status;
    fprintf (stderr, "the status is %d\n", status);
	return (status);
}
