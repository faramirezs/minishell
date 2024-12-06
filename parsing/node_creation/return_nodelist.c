/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   return_nodelist.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 13:33:37 by jslusark          #+#    #+#             */
/*   Updated: 2024/11/27 16:57:36 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
// change calloc to FT_CALLOC
// check if forbidden functions are here

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

bool add_redir(t_token_list *token, t_node	*new_node, int redir_i)
{
	if(redir_error(token))
		return(false);
	t_redir *new_redir = create_redir_data(token); // has to go here as its where we create the redirection
	if(!new_redir)
		return (false);
	new_redir->redir_i = redir_i;
	append_redir_data(&(new_node->redir_data), new_redir);
	redir_i++;
	return(true);
}
void	end_node(bool *node_starts, bool *found_cmd, t_node **head, t_node *new_node, t_token_list *token, int token_n) // a
{
	// redir_i = 0;
	printf(COLOR_RED"		%s\n"COLOR_RESET, token->value);
	printf(COLOR_RED"		TOKEN %d: PIPE %d\n"COLOR_RESET, token_n,  token->type);
	printf(COLOR_RED"		%s\n"COLOR_RESET, token->value);
	*node_starts = true;  // Node created, we have to start a new node
	*found_cmd = false; // As we start a new node we reset this flag to true
	append_node(head, new_node);// as we end the node we append it to our list
}

void add_cmd_and_args(bool *found_cmd, t_token_list *token, t_node *new_node, t_node *head, int token_n)
{
	if (!(*found_cmd)) // triggers command storing if true
	{
		printf(COLOR_BLUE"		TOKEN %d - "COLOR_RESET, token_n);
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
		printf(COLOR_BLUE"		TOKEN %d - "COLOR_RESET, token_n);
		printf("%s - %d", token->value, token->type); // he
		printf(COLOR_RED" (arg)\n"COLOR_RESET);
	}
}

t_node *node_init(int node_n, bool *node_starts) // nothing else needed
{
	printf(COLOR_RED"	- NODE %i: \n"COLOR_RESET, node_n);
	t_node *new_node = calloc(1, sizeof(t_node));
	if (!new_node) //not freeing things here as i will do in the main if error
	{
		printf("Minishell: Failed to allocate node number %d\n", node_n);
		return NULL;
	}
	//{
	// 	perror("Failed to allocate memory for a new node");
	// 	free_node_list(head); // Free the existing list
	// }
	new_node->node_i = node_n - 1;
	*node_starts = false; // Node has started
	return new_node;
}

bool grab_node(bool *pipe_at_start, bool *found_cmd, bool *node_starts, int node_n, int redir_i, t_token_list **token, t_node **head, t_node **new_node, int token_n)
{
	if (*node_starts)
	{
		*new_node = node_init(node_n, node_starts); // Initialize the new node
		if (!*new_node) // already printing error inside node_init
			return(false);
	}
	if ((*token)->type == PIPE) // Node ends at pipe
		end_node(node_starts, found_cmd, head, *new_node, *token, token_n);
	else // Process redirection, command, or arguments
	{
		*pipe_at_start = false;
		if ((*token)->type == REDIR_IN || (*token)->type == REDIR_OUT || (*token)->type == APPEND_OUT || (*token)->type == HEREDOC)
		{
			if (!add_redir(*token, *new_node, redir_i))
				return(false);
			*token = (*token)->next; // Advance token for the redirection target
			printf(COLOR_BLUE"		- REDIR STRUCT:\n"COLOR_RESET);
			printf(COLOR_BLUE"			TOKEN %d - Redirection:"COLOR_RESET, token_n);
			printf("%s - %d\n", (*token)->value, (*token)->type);
			if((*token)->type == HEREDOC) // the next token is seen as delimiter for the heredoc array
				printf(COLOR_BLUE"			TOKEN %d - delimiter:"COLOR_RESET, token_n);
			else // if redir is <, > and >> the next token is seen as file
				printf(COLOR_BLUE"			TOKEN %d - file:"COLOR_RESET, token_n);
			printf("%s\n", (*token)->value);
		}
		else // Handle commands and arguments
			add_cmd_and_args(found_cmd, *token, *new_node, *head, token_n);
	}
	*token = (*token)->next; // Move to the next token
	return(true);
}



// need 3 more funcs of 20/25 lines each
t_node *return_nodelist(t_token_list *token)
{
	int	node_n;						// We this to track the amount of nodes in a list and know if and how many times we have to pipe between nodes
	int	redir_i;					// redir index
	int token_n;
	bool node_starts;		// Flag to indicate if we have to start a node at start or after pipe
	bool found_cmd;		// Flag to make the first token a command of the node (unless redir data or pipe found) - this helps with cases like "> input.txt echo hello" result and "> input.txt hello echo" error
	bool pipe_at_start;  	// Flag to see if we start with a pipe, we set this to false the first token is not pipe.
	t_node *head; 			// First node in the list
	t_node *new_node;


	redir_i = 0;
	node_n = 1;
	token_n = 1;
	node_starts = true;
	found_cmd = false;
	pipe_at_start = true;
	head = NULL;

	printf(COLOR_GREEN"\nPARSING TOKENS...\n"COLOR_RESET);
	while (token)
		{
			if (unknown_token(token) || pipe_error(token, pipe_at_start))
				return NULL;
			if(!grab_node(&pipe_at_start, &found_cmd, &node_starts, node_n, redir_i, &token, &head, &new_node, token_n))
				return(NULL);
			token_n++;
		}
		if (new_node) // Append the last node if no pipe ends it
			append_node(&head, new_node);
	return (head);
}
