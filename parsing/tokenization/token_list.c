#include "../../headers/minishell.h"


t_token_list *init_token_list(t_scanner *scanner)
{
	t_token_list *new_token;
	t_token_list *head;    // Head of the linked list
	t_token_list *current;

	head = NULL;
	current = NULL;
	while (scanner_has_next(scanner))
	{
		scanner->next = scanner_next(scanner);
		new_token = OOM_GUARD(malloc(sizeof(t_token_list)), __FILE__, __LINE__);
		new_token->type = scanner->next.type;
		new_token->value = ft_strndup(scanner->next.lexeme.start, scanner->next.lexeme.length);
		new_token->next = NULL;
		if (!head)
			head = new_token; // First token becomes the head
		else
			current->next = new_token;
		current = new_token;
		printf("Token Type: %d, Token Value: %s\n", new_token->type, new_token->value);
		//print_token(scanner->next);
	}
	return (head); // Return the head of the linked list
}
