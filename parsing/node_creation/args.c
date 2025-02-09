/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 17:32:06 by alramire          #+#    #+#             */
/*   Updated: 2025/02/09 13:34:08 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/args.h"
#include "../../headers/minishell.h"

void	expand_args(t_token *token, t_args *args)
{
	char **temp;
	int i;
	//int j;

	i = 0;
	//printf("It entered to args_collector()\n");
	//fprintf(stderr, "DEBUG: Token received -> Length: %zu, Start: \"%.*s\"\n", token->lexeme.length, (int)token->lexeme.length, token->lexeme.start);

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

		//printf("DEB: arg is: %s\n", args->words[1]);

		//print_args (args);
		//check_null_array(args->words);
	}
	else
	{
		initialize_args(token, args);
	}
}

char	**copy_string_array(t_args *args)
{
	int		i;
	char	**new_array;

	new_array = OOM_GUARD(malloc(((*args->count) + 1) * sizeof(char *)),
			__FILE__, __LINE__);
	i = 0;
	while (args->words[i] != NULL)
	{
		new_array[i] = OOM_GUARD(ft_strdup(args->words[i]), __FILE__, __LINE__);
		i++;
	}
	if (args->words[i] == NULL)
		new_array[i] = NULL;
	free_args(&args);
	return (new_array);
}
