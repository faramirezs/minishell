/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 17:32:06 by alramire          #+#    #+#             */
/*   Updated: 2025/02/09 13:44:33 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/args.h"
#include "../../headers/minishell.h"

void	process_args(t_scanner *scanner, t_args *args)
{
	(*(args->count))++;
	args_collector(&scanner->next, args);
}

t_args	*initialize_args_count(void)
{
	t_args	*args;

	args = oom_guard(malloc(sizeof(t_args)), __FILE__, __LINE__);
	args->count = oom_guard(malloc(sizeof(int)), __FILE__, __LINE__);
	*(args->count) = 0;
	return (args);
}

void	initialize_args(t_token *token, t_args *args)
{
	args->words = oom_guard(malloc(2 * sizeof(char *)), __FILE__, __LINE__);
	args->words[0] = oom_guard(malloc((token->lexeme.length + 1)
				* sizeof(char)), __FILE__, __LINE__);
	ft_strlcpy(args->words[0], token->lexeme.start, token->lexeme.length + 1);
	args->words[1] = NULL;
}

void	args_collector(t_token *token, t_args *args)
{
	char	**temp;
	int		i;

	i = 0;
	if (*(args->count) > 1)
	{
		temp = oom_guard(malloc(((*(args->count)) + 1) * sizeof(char *)),
				__FILE__, __LINE__);
		while (i < *(args->count) - 1)
		{
			temp[i] = oom_guard(ft_strdup(args->words[i]), __FILE__, __LINE__);
			i++;
		}
		temp[i] = oom_guard(malloc((token->lexeme.length + 1) * sizeof(char)),
				__FILE__, __LINE__);
		ft_strlcpy(temp[i], token->lexeme.start, token->lexeme.length + 1);
		temp[i + 1] = NULL;
		free_string_array(&args->words);
		args->words = temp;
	}
	else
	{
		initialize_args(token, args);
	}
}

// char	**copy_string_array(t_args *args)
// {
// 	int		i;
// 	char	**new_array;

// 	new_array = oom_guard(malloc(((*args->count) + 1) * sizeof(char *)),
// 			__FILE__, __LINE__);
// 	i = 0;
// 	while (args->words[i] != NULL)
// 	{
// 		new_array[i] = oom_guard(ft_strdup(args->words[i]), __FILE__, __LINE__);
// 		i++;
// 	}
// 	if (args->words[i] == NULL)
// 		new_array[i] = NULL;
// 	free_args(&args);
// 	return (new_array);
// }
char **copy_string_array(t_args *args)
{
    char **new_array;
    int i;

    if (!args || !args->words)
        return NULL;

    // Count strings
    for (i = 0; args->words[i]; i++)
        ;

    printf("\033[33mDEBUG: Copying %d strings for exec node\033[0m\n", i);
    new_array = debug_malloc(sizeof(char *) * (i + 1), __FILE__, __LINE__);
    
    for (i = 0; args->words[i]; i++)
    {
        new_array[i] = debug_strdup(args->words[i], __FILE__, __LINE__);
        printf("\033[33mDEBUG: Copied arg[%d]: [%s] at %p\033[0m\n", 
               i, new_array[i], (void *)new_array[i]);
    }
    new_array[i] = NULL;

    return new_array;
}