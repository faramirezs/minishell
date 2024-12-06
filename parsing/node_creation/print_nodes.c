/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_nodes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 14:06:38 by jslusark          #+#    #+#             */
/*   Updated: 2024/11/26 16:49:32 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

char *return_token_enum(int token_type)
{
	if (token_type == BUILT_IN)
		return "BUILT_IN"; // Commands like echo, cd, pwd, etc.
	else if (token_type == ENV_VAR)
		return "ENV_VAR"; // Environment variables starting with $ (e.g., $HOME)
	else if (token_type == ABS_PATH)
		return "ABS_PATH"; // Absolute path starting with /
	else if (token_type == REL_PATH)
		return "REL_PATH"; // Relative path starting with ./ or ../
	else if (token_type == OPTION)
		return "OPTION"; // Options like -n for echo
	else if (token_type == REDIR_IN)
		return "REDIR_IN (<)"; // Input redirection ("<")
	else if (token_type == REDIR_OUT)
		return "REDIR_OUT (>)"; // Output redirection (">")
	else if (token_type == APPEND_OUT)
		return "APPEND_OUT (>>)"; // Append redirection (">>")
	else if (token_type == HEREDOC)
		return "HEREDOC (<<)"; // Here-document redirection ("<<")
	else if (token_type == PIPE)
		return "PIPE"; // Pipe operator ("|")
	else if (token_type == STRING_D_QUOTES)
		return "STRING_D_QUOTES"; // Strings inside double quotes
	else if (token_type == STRING_S_QUOTES)
		return "STRING_S_QUOTES"; // Strings inside single quotes
	else if (token_type == WORD)
		return "WORD"; // Any letter or number that is not quoted
	else if (token_type == UNKNOWN)
		return "UNKNOWN"; // Invalid token (e.g., \, ;, &&, ||, etc.)
	else
		return "INVALID_ENUM"; // For invalid enum values
}

char *return_target_enum(int redir_target_type)
{
	if(redir_target_type == TARGET_FILENAME)
		return("TARGET_FILENAME");
	if(redir_target_type == TARGET_PATHNAME)
		return("TARGET_PATHNAME");
	if(redir_target_type == TARGET_ENV_PATHNAME)
		return("TARGET_ENV_PATHNAME");
	return("TARGET_DELIMITER ");
}


void print_redir_data(t_redir *head)
{
	if (!head)
	{
		printf(COLOR_RED"		- t_redir redir_data:\n"COLOR_RESET);
		printf("		(NULL)\n");
		return;
	}
	t_redir *current = head;
	printf(COLOR_RED"		- t_redir redir_data:\n"COLOR_RESET);
	while (current)
	{
		printf(COLOR_BLUE"				- REDIR N %d:\n"COLOR_RESET, current->redir_i + 1);
		printf("			  - int redir_type: %s\n", return_token_enum(current->redir_type));

		printf("			  - char *target: %s (%s)\n", current->target, return_token_enum(current->target_token_type));
		printf("			  - int target_type: %s\n", return_target_enum(current->target_type));


		// printf("			  - int target_type: %d\n", current->target_type);
		// if (current->arg_value)
		// 	printf("			  - char *value: %s\n", current->arg_value);
		// else
		// 	printf("			  - char *value: (NULL)\n");
		current = current->next; // Move to the next argument in the list
	}
}

void print_args(t_args *head)
{
	if (!head)
	{
		printf(COLOR_RED"		- t_args cmd_args:\n"COLOR_RESET);
		printf("		(NULL)\n");
		return;
	}
	// printf("HEYYYY\n");
	t_args *current = head;
	int index = 1; // Index to track the argument position in the list
	printf(COLOR_RED"		- t_args cmd_args:\n"COLOR_RESET);
	while (current)
	{
		printf(COLOR_BLUE"			ARG N_%d\n"COLOR_RESET, index);
		printf("			  - int type: %s\n", return_token_enum(current->arg_type));
		if (current->arg_value)
			printf("			  - char *value: %s\n", current->arg_value);
		else
			printf("			  - char *value: (NULL)\n");
		current = current->next; // Move to the next argument in the list
		index++;
	}
}

void print_nodes(t_node *head)
{
	int n = 0;

	printf(COLOR_GREEN"\nRETURNING NODE LIST...\n"COLOR_RESET); // Print the node_i value
	t_node *curr = head; // Start with the head of the node list
	while (curr != NULL)
	{
		n++;
		printf(COLOR_RED"	- NODE %d\n"COLOR_RESET, n); // Print the node_i value
		printf("		- int node_i: %d\n", curr->node_i); // Print the node_i value
		printf(COLOR_RED"		- t_cmd command_data:\n"COLOR_RESET); // Print the node_i value
		if (curr->cmd_data)
		{
			printf("			- char *cmd_value: %s\n", curr->cmd_data->cmd_value); // Print the node_i value
			printf("			- int cmd_type: %s\n", return_token_enum(curr->cmd_data->cmd_type)); // Print the node_i value
		}
		if (!curr->cmd_data)
			printf("		(NULL)\n"); // Print the node_i value
		print_args(curr->cmd_args);
		print_redir_data(curr->redir_data);
		curr = curr->next; // Move to the next node
	}
	printf("\n	Total number of pipes: %d\n", n - 1); // Print the node_i value
	printf("	Total number of nodes: %d\n", n); // Print the node_i value
}