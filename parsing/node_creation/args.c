/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   args.c											 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: alejandroramirez <alejandroramirez@stud	+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2025/01/10 17:32:06 by alramire		  #+#	#+#			 */
/*   Updated: 2025/01/15 15:50:29 by alejandrora	  ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "../../headers/args.h"
#include "../../headers/minishell.h"

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

		temp = OOM_GUARD(malloc(((*(args->count)) + 1) * sizeof(char *)), __FILE__, __LINE__);
		while(i < *(args->count) - 1)
		{
			temp[i] = OOM_GUARD(ft_strdup(args->words[i]), __FILE__, __LINE__);
			i++;
		}
		temp[i] = OOM_GUARD(malloc((token->lexeme.length + 1) * sizeof(char)), __FILE__, __LINE__);
		ft_strlcpy(temp[i], token->lexeme.start, token->lexeme.length + 1);
		temp[i + 1] = NULL;
		//check_null_array(args->words);
		free_string_array(&args->words);
		args->words = temp;
		//check_null_array(args->words);
	}
	else
	{
		// Here, always args count is equal to 1
		args->words = OOM_GUARD(malloc(2 * sizeof(char *)), __FILE__, __LINE__);
		args->words[0] = OOM_GUARD(malloc((token->lexeme.length + 1) * sizeof(char)), __FILE__, __LINE__);
		ft_strlcpy(args->words[0], token->lexeme.start, token->lexeme.length + 1);
		// printf("Token lexeme length: %zu in file %s at line %d\n", token->lexeme.length, __FILE__, __LINE__);
		// printf("Last string from words is %s\n", args->words[(*(args->count)) - 1]);
		// printf("in file %s at line %d\n", __FILE__, __LINE__);
		args->words[1] = NULL;
		//check_null_array(args->words);
	}
}

char **copy_string_array(t_args *args)
{
	int i;
	char **new_array;

	new_array = OOM_GUARD(malloc(((*args->count) + 1) * sizeof(char *)), __FILE__, __LINE__);
	i = 0;
	while (args->words[i] != NULL)
	{
		new_array[i] = OOM_GUARD(ft_strdup(args->words[i]), __FILE__, __LINE__);
		i++;
	}
	if (args->words[i] == NULL)
		new_array[i] = NULL;
	free_args(&args);
	//check_null_array(new_array);
	return (new_array);
}

void free_string_array(char ***array)
{
	int i;

	if (array && *array)
		{
			//printf("Clearing words\n");
			i = 0;
			while ((*array)[i] != NULL)
			{
				free((*array)[i]);
				//(*array)[i] = NULL;
				i++;
			}
			free((*array));
			(*array) = NULL;
		}
}

void free_args(t_args **args)
{
	if (args && *args)
	{
		if ((*args)->words)
		{
			i = 0;
			while (i < *((*args)->count))
			{
				free((*args)->words[i]);
				(*args)->words[i] = NULL;
				i++;
			}
			free((*args)->words);
			(*args)->words = NULL;
		}
		if ((*args)->count)
		{
			free((*args)->count);
			(*args)->count = NULL;
		}
		free(*args);
		*args = NULL;
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

void print_array(char **array)
{
	int	i;

	printf("Print_array\n");
	i = 0;
	while (array[i] != NULL)
	{
		printf("%p word %d: %s\n", &array[i], i + 1, array[i]);
		i++;
	}
	if(array[i] == NULL)
		printf("%p, array[%d] is NULL\n", &array[i], i);
	else
		printf("array is not NULL-terminated\n");
}

void check_null_array(char **array)
{
	int i;

	i = 0;

	while(array[i] != NULL)
	{
		printf("%p: array[%d] is not NULL\n", &array[i], i);
		i++;
	}
	if (array[i] == NULL)
		printf("%p: array[%d] is NULL\n", &array[i], i);
}
