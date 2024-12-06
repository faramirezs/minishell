#ifndef TOKEN_LIST_H
#define TOKEN_LIST_H

#include "char_itr.h"
#include "token.h"
#include "guards.h"
#include "scanner.h"

typedef struct s_token_list
{
	t_token_type type;         // just type,  Type of the token, e.g., COMMAND, STRING_LITERAL, etc.
	char *value;      // just value, Value of the token, e.g., "echo", "hello"
	struct s_token_list *next; // Just next, Pointer to the next token in the linked list
} t_token_list;

t_token_list *init_token_list(t_scanner *self);

#endif

