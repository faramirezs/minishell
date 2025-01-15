/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 16:26:03 by jslusark          #+#    #+#             */
/*   Updated: 2024/12/17 17:30:21 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/minishell.h"
//#include "include/node.h"
//#include "include/parser.h"

/*
int	main(int argc, char **argv)
{
	char			*input;
	t_char_itr		itr;
	t_scanner		scanner;
	t_tree_node		*tree_node;
	(void)argc;
	(void)argv;

	while (1)
	{
		input = readline(COLOR_GREEN "Minishell> " COLOR_RESET);
		if (ft_strlen(input) > 0)
		{
			add_history(input);
			itr = char_itr_value(input, ft_strlen(input));
			scanner = scanner_value(itr);
			tree_node = parse_tree_node(&scanner);
			exec(tree_node);
			//visit_node(tree_node, 0);
		}
		if(!tree_node)
		{
			free(input);
			//free_mock_tokens(tokens);
		}
		if(ft_strcmp(input, "exit") == 0) //<---- we will have to remove this as we handle exit in the execution bit
		{
			free(input);
			//free_mock_tokens(tokens);
			clear_history();
			exit(0);
		}
		else
		{
			//node_list = return_nodelist(tokens); //
			//free_mock_tokens(tokens); // we free the token list since we allocated and created our nodes
			free(input); // we also free the input as we have processed it already as tokens and then nodes
			 if(!node_list)
			{
				// planning that every error (not only allocation error but also parsing error) makes the parsing list "NULL"
				// so that we just need to free once in here and not on every parsing considion with meet
				// could also return an error, but need to check
				// printf("Error increating node! (I could possibly return the error here too..?)\n"); // prob will print in the fuction, i will just free here
				free_node_list(node_list); // for now this is seen as double free because I al already freeing in every scenario i find in my parsing function
			}
			else
			{
				// print node logic will be added here just to help us with error handling, we can comment it our or remove it once we submit the project
				print_nodes(node_list); // let's leave this function for error handling, we can comment it our or remove it once we submit the project
				exec_node(node_list); // I already set a simple ground for initiating execution from our nodes
				free_node_list(node_list);// we have to free the node_list when finished executing it and ready to prompt a new input
			} 
		}
	}
	clear_history(); //Before I had this: rl_clear_history(); shouldn't this run in the while loop? (outside if statements)
	return (0);
}
*/

int	main(int argc, char **argv)
{
	char			*input;
	t_char_itr		itr;
	t_scanner		scanner;
	t_tree_node		*tree_node;
	(void)argc;
	(void)argv;

	while (1)
	{
		input = readline(COLOR_GREEN "Minishell> " COLOR_RESET);
		if (!input) // Handle Ctrl+D
			break;

		if (ft_strlen(input) > 0)
		{
			add_history(input);
			itr = char_itr_value(input, ft_strlen(input));
			scanner = scanner_value(itr);
			tree_node = parse_tree_node(&scanner);

			// Check for builtins
			if (tree_node && tree_node->data.exec_u)
			{
				char **args = parse_command(tree_node->data); // Extract args from tree_node
				if (args && execute_builtin(args) == 0) // Check if builtin executed
				{
					free(args); // Free args only if builtin ran
					free_tree_node(tree_node); // Free the tree_node
					free(input);
					continue; // Go back to prompt
				}
				free(args);
			}

			// Execute external commands
			if (tree_node)
				exec(tree_node); // This handles non-builtin commands
		}

		free_tree_node(tree_node); // Free the tree_node after execution
		free(input); // Free the input string
	}
	clear_history(); // Clears readline history
	return (0);
}