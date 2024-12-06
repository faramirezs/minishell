/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   old_print.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 13:33:37 by jslusark          #+#    #+#             */
/*   Updated: 2024/11/25 12:54:20 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
The current parsing function doesn't actually build nodes yet.
Instead, it is designed to analyze and show how Bash interprets the input we pass to it and create our nodes.
I think this is essential for us so that we can both agree on how things will work before we build our data structures and execution functions.
I left some notes for you in a folder called how_bash_parses_tokens:
	- bash_parse_explained.c
	- parsing_rules.c

I decided to do this so that we can already see a structure and understand how to progress within our project, improve our code and avoid special
cases that will make things harder for us to fix. It's been a bit hard at the beginning but I feel that i needed to do this
to avoid impulsive decisions in building our parsing structure and finding myself with a mess that would make things more confusing for all of us.

I just need to add some other small details to this code today.
Once i am done I will use this same logic to build our nodes and another file called print_nodes.c
which will print the nodes that I created.
 */

#include "../../headers/minishell.h"

t_node *parse(t_token_list *mock_token)
{
	int node_n = 1;					// We may need this for knowing how many nodes we have for piping and forking? prob also for malloc?
	int token_n = 0;				// We may not need this
	bool start_node = true;			// Flag to indicate if we have to start a node at start or after pipe
	bool get_command = true;		// Flag to make the first token a command of the node (unless redir data or pipe found) - this helps with cases like "> input.txt echo hello" result and "> input.txt hello echo" error
	bool pipe_at_start = true;  	// Flag to see if we start with a pipe, we set this to false the first token is not pipe.

	while (mock_token) // We iterate through every token in our list
	{
		token_n++; // use to print the position of the token in our node - start from 0 instead than 1 perhaps?
		if (start_node == true) // flag that will make us start
		{
			printf(COLOR_RED"	- NODE %i: \n"COLOR_RESET, node_n);
			start_node = false; // we started the node so what happens next is grabbing the node data
		}
		if (mock_token->type == UNKNOWN) // interrupts node creation as it checks invalid tokens: symbols outside of string like (&&, ;, \, /, ?, unclosed ", unclosed ', ~, and * etc..) <- can we avoid tokens
		{
			printf("Error: invalid tokens \n");
			break;
		}
		if (mock_token->type == PIPE) // checks pipe to end/start node and command or trigger error
		{
			if (!mock_token->next || pipe_at_start || mock_token->next->type == PIPE) // interrupts node creation
			{
				printf("Error: cannot start/end pipe and cannot have consecutive pipes\n"); // Starting with pipe is error and we do not have to handle when a command ends with pipe
				break;
			}
			else // we end the node, flag to start a new one and its command
			{
				printf(COLOR_RED"		%s\n"COLOR_RESET, mock_token->value);
				printf(COLOR_RED"		TOKEN %d: PIPE %d\n"COLOR_RESET, token_n,  mock_token->type);
				printf(COLOR_RED"		%s\n"COLOR_RESET, mock_token->value);
				start_node = true;  // Node created, we have to start a new node
				get_command = true; // As we start a new node we reset this flag to true
				node_n++;			// As we start a new node we increase the node number,
			}
		}
		else //sets pipeflag to false as if pipe is not 1st then it's not an error
		{
			pipe_at_start = false;
			//if redir grab redir and file data
			if (mock_token->type == REDIR_IN || mock_token->type == REDIR_OUT || mock_token->type == APPEND_OUT || mock_token->type == HEREDOC)
			{
				int redir_type = mock_token->type;
				printf(COLOR_BLUE"		- REDIR STRUCT:\n"COLOR_RESET);
				printf(COLOR_BLUE"			TOKEN %d - Redirection:"COLOR_RESET, token_n);
				printf("%s - %d\n", mock_token->value, mock_token->type);
				// Trigger error if token after redir is NULL - Redir has to be followed by something that shell will see as file
				if(mock_token->next == NULL)
				{
					printf(COLOR_RED"ERROR: redirections need to be followed by a file! \n"COLOR_RESET);
					break; // we break the loop
				}
				// If we have a token after redir, trigger error if redir symbol is followed by redir symbol or pipe, if not we grab the file
				if(mock_token->next != NULL)
				{
					token_n++;
					mock_token = mock_token->next; // me move to the ne
					if (mock_token->type == PIPE || mock_token->type == REDIR_IN
						|| mock_token->type == REDIR_OUT || mock_token->type == APPEND_OUT
						|| mock_token->type == HEREDOC) // if redir symbol is followed by |, >, >>, < and << this is considered an error
					{
						printf(COLOR_RED"ERROR: redirection cannot be immediately followed by pipes or other redirections \n"COLOR_RESET);
						break; // we break the loop as there is no point in continuing to build a node if it's an error
					}
					else
					{
						if(redir_type == HEREDOC) // the next token is seen as delimiter for the heredoc array
							printf(COLOR_BLUE"			TOKEN %d - delimiter:"COLOR_RESET, token_n); // this is either
						else // if redir is <, > and >> the next token is seen as file
							printf(COLOR_BLUE"			TOKEN %d - file:"COLOR_RESET, token_n);
						printf("%s - %d\n", mock_token->value, mock_token->type);
					}
				}
			}
			else // everything that is not redir is either node command or args
			{
				printf(COLOR_BLUE"		TOKEN %d:"COLOR_RESET, token_n);
				if (get_command) // triggers command storing if true
				{
					printf("%s - %d", mock_token->value, mock_token->type);
					printf(COLOR_RED" (command)\n"COLOR_RESET);
					get_command = false; // command found, next tokens are args
				}
				else // triggers args storing
				{
					printf("%s - %d", mock_token->value, mock_token->type); // he
					printf(COLOR_RED" (arg)\n"COLOR_RESET);
				}
			}
		}
		mock_token = mock_token->next; // Move to the next token
	}
	// Summary of AST structure
	// if (node_n == 1)
	// {
	// 	printf("\n Our TREE is made of just one node:\n a node is a command followed by arguments (strings, redirections, options, etc.)\n");
	// } else if (node_n > 1) {
	// 	printf("\n Our TREE is made of %d nodes connected by pipes within a single node table.\n", node_n);
	// }
	return NULL;
}
