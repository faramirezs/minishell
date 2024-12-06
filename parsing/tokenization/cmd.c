#include "../../headers/minishell.h"

int compare_token(const t_token *token, const char *str)
{
	return strncmp(token->lexeme.start, str, token->lexeme.length);
}

void check_cmd(t_scanner *self)
{
	size_t i;

	i = 0;
	while (i < self->next.lexeme.length)
	{
		i++;
	}
	if (compare_token(&self->next, "echo") == 0 ||
		compare_token(&self->next, "unset") == 0 ||
		compare_token(&self->next, "cd") == 0 ||
		compare_token(&self->next, "pwd") == 0 ||
		compare_token(&self->next, "export") == 0 ||
		compare_token(&self->next, "env") == 0)
	{
		self->next.type = BUILT_IN;
	}
	else
	{
		self->next.type = WORD;
	}
}
