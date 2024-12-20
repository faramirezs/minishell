#include "../../headers/args.h"
#include "../../headers/minishell.h"

//Where this function is being called, we need to initialize word count to 1.
void args_collector(t_token *token, t_args *args)
{
	char **temp;
	int i;
	//int j;

	i = 0;
	//printf("It entered to args_collector()\n");
	//printf("args->count is %i\n", *(args->count));
	if (*(args->count) > 1)
	{
		temp = OOM_GUARD(malloc(*(args->count)* sizeof(char *)), __FILE__, __LINE__);
		while(i < *(args->count) - 1)
		{
			temp[i] = args->words[i];
			i++;
		}
		temp[(*(args->count)) - 1] = OOM_GUARD(malloc(token->lexeme.length * sizeof(char)), __FILE__, __LINE__);
		ft_strlcpy(temp[(*(args->count)) - 1], token->lexeme.start, token->lexeme.length);
		//printf("Token lexeme length: %zu in file %s at line %d\n", token->lexeme.length, __FILE__, __LINE__);
		//printf("Last string from temp is %s\n", temp[(*(args->count)) - 1]);
		//printf("in file %s at line %d\n", __FILE__, __LINE__);
		free(args->words);
		args->words = temp;
	}
	else
	{
		// Here, always args count is equal to 1
		args->words = OOM_GUARD(malloc(*(args->count) * sizeof(char *)), __FILE__, __LINE__);
		args->words[(*(args->count)) - 1] = OOM_GUARD(malloc(token->lexeme.length * sizeof(char)), __FILE__, __LINE__);
		ft_strlcpy(args->words[(*(args->count)) - 1], token->lexeme.start, token->lexeme.length);
		// printf("Token lexeme length: %zu in file %s at line %d\n", token->lexeme.length, __FILE__, __LINE__);
		// printf("Last string from words is %s\n", args->words[(*(args->count)) - 1]);
		// printf("in file %s at line %d\n", __FILE__, __LINE__);
	}
}


char **copy_string_array(char **array, int count)
{
	// Allocate memory for the new array of pointers
	char **new_array = OOM_GUARD(malloc(count * sizeof(char *)), __FILE__, __LINE__);
	int i;
	// Copy each string
	i = 0;
	while (i < count)
	{
		if (array[i] != NULL)
		{
			new_array[i] = OOM_GUARD(ft_strdup(array[i]), __FILE__, __LINE__); // Duplicate the string
		}
		else
		{
			new_array[i] = NULL; // Handle NULL pointers if the input array has them
		}
		i++;
	}
	free_string_array(new_array);
	return (new_array);
}

void free_string_array(char **array)
{
	int i;

	if (array)
		{
			printf("Clearing words\n");
			i = 0;
			while (i < *(array) - 1)
			{
				free(array[j]);
				i++;
			}
			free(array);

		}
}
void print_args(t_args *args)
{
	int	i;

	printf("Count: %d\n", *(args->count));
	i = 0;
	while (i < *(args->count))
	{
		printf("Word %d: %s\n", i + 1, args->words[i]);
		i++;
	}
}
