#ifndef ARGS_H
#define ARGS_H

#include "char_itr.h"
#include "token.h"
#include "scanner.h"

typedef struct s_args
{
	int *count;
	char **words;
} t_args;
//This strcuts needs to be emptied after the exec run.


void args_collector(t_token *token, t_args *args);


#endif
