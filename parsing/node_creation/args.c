#include "../../headers/args.h"
#include "../../headers/minishell.h"

//Where this function is being called, we need to initialize word count to 1.
void args_collector(t_token *token, t_args *args)
{
	char **temp;
	int i;

	i = 0;
	printf("It entered to args_collector()\n");
	printf("args->count is %i\n", *(args->count));
	if (*(args->count) > 1)
	{
		temp = OOM_GUARD(malloc(*(args->count)* sizeof(char *)), __FILE__, __LINE__);
		while(i < *(args->count) - 1)
		{
			temp[i] = args->words[i];
			i++;
		}
		temp[*(args->count)] = OOM_GUARD(malloc(token->lexeme.length * sizeof(char)), __FILE__, __LINE__);
		ft_strlcpy(temp[*(args->count)], token->lexeme.start, token->lexeme.length);
		printf("Last string from temp is %s\n", temp[*(args->count)]);
		free(args->words);
		args->words = temp;
	}
	else
	{

		//Here, always args count is equal to 1
		args->words = OOM_GUARD(malloc(*(args->count)* sizeof(char *)), __FILE__, __LINE__);
		args->words[(*(args->count)) - 1] = OOM_GUARD(malloc(token->lexeme.length * sizeof(char)), __FILE__, __LINE__);
		ft_strlcpy(args->words[(*(args->count)) - 1], token->lexeme.start, token->lexeme.length);
		printf("Token lexeme start: %s, length: %zu\n", token->lexeme.start, token->lexeme.length);
		printf("Last string from words is %s\n", args->words[*(args->count)]);
		//the case there the words array is empty...?
		//
	}
}


