#ifndef ARGS_H
#define ARGS_H

#include "minishell.h"



//This strcuts needs to be emptied after the exec run.


void args_collector(t_token *token, t_args *args);
char **copy_string_array(t_args *args);
void free_string_array(char ***array);
void free_args(t_args **args);
void print_args(t_args *args);
void check_null_array(char **array);
void print_array(char **array);

#endif
