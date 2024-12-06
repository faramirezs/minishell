/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_logic.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 13:33:37 by jslusark          #+#    #+#             */
/*   Updated: 2024/11/26 19:30:26 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

bool	unknown_token(t_token_list *token)
{
		if (token->type == UNKNOWN) // ERROR AND FREE
		{
			printf("Minishell: found invalid token %s\n", token->value);
			return(true);
		}
		return(false);
}

bool	pipe_error(t_token_list *token, bool pipe_at_start)
{
	if (token->type == PIPE) // ERROR AND FREE
	{
		if (!token->next) // Check if the next token is NULL
		{
			printf("Minishell: found `%s' at end of syntax\n", token->value);
			return(true);
		}
		else if (pipe_at_start || token->next->type == PIPE) // Ensure token->next is not NULL
		{
			printf("Minishell: syntax error near unexpected token `%s'\n", token->value);
			return(true);
		}
	}
	return(false);
}
bool	redir_error(t_token_list *token)
{
	if (token->type == REDIR_IN || token->type == REDIR_OUT || token->type == APPEND_OUT || token->type == HEREDOC)
	{
		if(token->next == NULL) // error and free
		{
			printf("Minishell: syntax error near unexpected token `newline'\n");
			return(true); // we break the loop
		}
		else if (token->next->type == PIPE || token->next->type == REDIR_IN
			|| token->next->type == REDIR_OUT || token->next->type == APPEND_OUT
			|| token->next->type == HEREDOC || token->next->type == UNKNOWN)
		{
			printf("Minishell: syntax error near unexpected token `%s'\n", token->next->value);
			return(true); // we break the loop as there is no point in continuing to build a node if it's an error
		}
	}
	return(false);
}

bool add_redir(t_token_list *token, int redir_i, t_node	*new_node)
{
	// if (token->type == REDIR_IN || token->type == REDIR_OUT || token->type == APPEND_OUT || token->type == HEREDOC)
	// {
		if(redir_error(token))
			return(false);
		t_redir *new_redir = create_redir_data(token); // has to go here as its where we create the redirection
		if(!new_redir)
			return (false);
		new_redir->redir_i = redir_i;
		append_redir_data(&(new_node->redir_data), new_redir);
		redir_i++;
		// token = token->next; // me move to the next token to check
		// token_n++;
		// token = token->next; // me move to the next token to check
		// printf(COLOR_BLUE"		- REDIR STRUCT:\n"COLOR_RESET);
		// printf(COLOR_BLUE"			TOKEN - Redirection:"COLOR_RESET);
		// printf("%s - %d\n", token->value, token->type);
		// token = token->next; // me move to the next token to check
		// // token_n++;
		// if(token->type == HEREDOC) // the next token is seen as delimiter for the heredoc array
		// 	printf(COLOR_BLUE"			TOKEN - delimiter:"COLOR_RESET);
		// else // if redir is <, > and >> the next token is seen as file
		// 	printf(COLOR_BLUE"			TOKEN - file:"COLOR_RESET);
		// printf("%s\n", token->value);
		return(true);
	// }
	// return(true);
}
void	end_node(bool *node_starts, bool *found_cmd, t_node **head, t_node *new_node)
{
	// redir_i = 0;
	*node_starts = true;  // Node created, we have to start a new node
	*found_cmd = false; // As we start a new node we reset this flag to true
	append_node(head, new_node);// as we end the node we append it to our list
}

void add_cmd_and_args(bool *found_cmd, t_token_list *token, t_node *new_node, t_node *head)
{
	if (!(*found_cmd)) // triggers command storing if true
	{
		printf("%s - %d", token->value, token->type);
		printf(COLOR_RED" (command)\n"COLOR_RESET);
		add_cmd_to_node(head, new_node, token); // had to return as error
		*found_cmd = true; // command found, if we have other tokens they are args if not redir data
		// ADD ECHO -N FLAG HERE, if mocktoken->next-token_type == OPTION
	}
	else // triggers args storing
	{
		t_args *new_arg = create_newarg_data(head, token);
		append_newarg_to_cmdargs(&(new_node->cmd_args), new_arg); // Pass cmd_args as a double pointer
		printf("%s - %d", token->value, token->type); // he
		printf(COLOR_RED" (arg)\n"COLOR_RESET);
	}
}

t_node *node_init(t_node *head, int node_n, bool *node_starts)
{
	printf(COLOR_RED"	- NODE %i: \n"COLOR_RESET, node_n);
    t_node *new_node = calloc(1, sizeof(t_node));
    if (!new_node)
    {
        perror("Failed to allocate memory for a new node");
        free_node_list(head); // Free the existing list
        return NULL;
    }
    new_node->node_i = node_n - 1;
    *node_starts = false; // Node has started
    return new_node;
}



// need 3 more funcs of 20/25 lines each
t_node *return_nodelist(t_token_list *token)
{
	int	node_n;						// We this to track the amount of nodes in a list and know if and how many times we have to pipe between nodes
	int	token_n;					// We may not need this
	int	redir_i;					// redir index
	bool node_starts;		// Flag to indicate if we have to start a node at start or after pipe
	bool found_cmd;		// Flag to make the first token a command of the node (unless redir data or pipe found) - this helps with cases like "> input.txt echo hello" result and "> input.txt hello echo" error
	bool pipe_at_start;  	// Flag to see if we start with a pipe, we set this to false the first token is not pipe.
	t_node *head; 			// First node in the list
	t_node *new_node;


	redir_i = 0;
	node_n = 1;
	token_n = 0;
	node_starts = true;
	found_cmd = false;
	pipe_at_start = true;
	head = NULL;

	printf(COLOR_GREEN"\nPARSING TOKENS...\n"COLOR_RESET);
	while (token)
	{
		token_n++;
		if(unknown_token(token) || pipe_error(token, pipe_at_start))
			return(NULL);
		if (node_starts)
		{
			new_node = node_init(head, node_n, &node_starts);
            if (!new_node)
                return NULL;
		}
		if (token->type == PIPE) // if we don't have pipe errors it means our node ends and we ahve to start anothee one
		{
				printf(COLOR_RED"		%s\n"COLOR_RESET, token->value);
				printf(COLOR_RED"		TOKEN %d: PIPE %d\n"COLOR_RESET, token_n,  token->type);
				printf(COLOR_RED"		%s\n"COLOR_RESET, token->value);
				node_n++;
				redir_i = 0;
				end_node(&node_starts, &found_cmd, &head, new_node);
		}
		else // if we don't hit PIPE or UNKNOWN, se set pipe at sratrt false to process other tokens (redir, cmd and args)
		{
			pipe_at_start = false;
			// if(!add_redir(token, redir_i, new_node))
			// 	return(NULL);
			if (token->type == REDIR_IN || token->type == REDIR_OUT || token->type == APPEND_OUT || token->type == HEREDOC)
			{
				if(!add_redir(token, redir_i, new_node))
					return(NULL);
				token = token->next; // me move to the next token to check
				// printf(COLOR_BLUE"		- REDIR STRUCT:\n"COLOR_RESET);
				// printf(COLOR_BLUE"			TOKEN %d - Redirection:"COLOR_RESET, token_n);
				// printf("%s - %d\n", token->value, token->type);
				// token_n++;
				// if(token->type == HEREDOC) // the next token is seen as delimiter for the heredoc array
				// 	printf(COLOR_BLUE"			TOKEN %d - delimiter:"COLOR_RESET, token_n);
				// else // if redir is <, > and >> the next token is seen as file
				// 	printf(COLOR_BLUE"			TOKEN %d - file:"COLOR_RESET, token_n);
				// printf("%s\n", token->value);
			}
			else // CHECKS THE REST (cmd and args)
			{
				printf(COLOR_BLUE"		TOKEN %d:"COLOR_RESET, token_n);
				add_cmd_and_args(&found_cmd, token, new_node, head);
			}
		}
		token = token->next; // Move to the next token
	}
	append_node(&head, new_node);// we also have to append the node when it doesn't have pipe
	printf("	Total number of nodes detected: %d\n", node_n);
	return (head);
}
