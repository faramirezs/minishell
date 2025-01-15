#include "../headers/built_in.h"
#include "../headers/structs.h"
/*
int execute_command(char **av, bld_in *builtins)
{
	bld_in  *builtin;
	
	if (av[0] == NULL)
		return (1);
	builtin = find_builtin(builtins, av[0]);
	if (builtin)
		builtin->func(av);
	if (execvp(av[0], av) == -1)
	{
		fprintf (stderr, "Mestepum: command not found: %s\n", av[0]);
		return (127);
	}
	return (0);
}
*/
bld_in  *find_builtin(bld_in *head, const char *command)
{
	while (head != NULL)
	{
		if (strcmp (head->name, command) == 0)
			return (head);
		head = head->next;
	}
	return (NULL);
}

char	**ms_matrix_add_line(char **matrix, char *new_line)
{
	int		i;
	char	**new_matrix;

	i = 0;
	while (matrix[i])
		i++;
	new_matrix = malloc(sizeof(char *) * (i + 2));
	i = 0;
	while (matrix[i])
	{
		new_matrix[i] = ft_strdup(matrix[i]);
		i++;
	}
	new_matrix[i] = ft_strdup(new_line);
	i++;
	new_matrix[i] = NULL;
	ft_free_tab(matrix);
	return (new_matrix);
}
