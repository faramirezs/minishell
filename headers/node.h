/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 12:34:31 by jslusark          #+#    #+#             */
/*   Updated: 2025/01/10 17:11:59 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NODE_H
# define NODE_H

#include "token_list.h"

/* typedef struct s_token_list
{
	int type;         // Type of the token, e.g., COMMAND, STRING_LITERAL, etc.
	char *value;      // Value of the token, e.g., "echo", "hello"
	struct s_token_list *next; // Pointer to the next token in the linked list
}			t_token_list; */



typedef struct s_redir
{
	struct s_redir *prev; // we need this so that the curr redir can communicate with the previous if a node had more than 1 redir
	int		redir_type;   // Type of redirection (REDIR_IN, REDIR_OUT, APPEND_OUT, hEREDOC)
	int		redir_i; // can be useful when we have more than 1 redirection in a node
	char	*target;		// Target is filename (for input, output and append) or delimiter (for HEREDOC)
	int		target_type;	// can be delimiter for heredoc, for else it can be file name or a valid path to a file (abs, relative or env_var like $PATH)
	int		target_token_type; // We store the target token type so we can use to understand what path is used in TARGET PATH_NAME
	struct s_redir *next; // we need this as a redirection can be followed by other redirections and interact with them

	// this input can be stored after we have the nodes and before/while executing - still nedd to check if good
	char	*exec_path; // the path where we have to execute the redir, comes from target if target_type is path or we assign it by default to home of shell
	char	*exec_file; // the file we have to execute on (or also create in some cases), comes from target, if target type is not word we have to get the last word in path (comes after last /)
	char	*file_input; // we can store everything together and then split in different lines after if input has \n
	char	**split_input; // we can store everything together and then split in different lines after if input has \n
	int flags; // flags that we pass to open(), depends on the redir type
	int fd; // we can store fd here for future use?
	bool close_fd; // Indicates if the fd should be closed after use.

} t_redir;

typedef struct s_args // list of tokens that follow the command token in the node and considered its arguments
{
	struct s_args *prev;   // Pointer to the prev argument in the list (adding it in case it can be useful in the future)
	int arg_type;         //anything that is not a command/builtin token or part of the redirection struct (file, redir type)
	char *arg_value;      // Value of the token (strings.. could it be also expandables and env variables?)
	struct s_args *next;   // Pointer to the next argument in the list
} t_args;

typedef struct s_cmd // the the first token thas is not redirection data (redirection symbol and file name) or pipe is seen by bash as the command of the node
{
	int		cmd_type;         // type of the token given us from the tokenizer, this will help us with execution and error handling (if the command is not an actual command)
	char	*cmd_value;       // what is the value of the commmand (echo, cd, exit, string_value etc)
} t_cmd;

//a node table is a linked list of nodes, we traverse each node as a double linked list.. why?
// we do it so that when we have to execure we can access each node
typedef struct s_node // A node typically has this data: command, command arguments, redirection data (redir symbol and file name) and pipe, a sequence of node commands is called command_list
{
	struct s_node *prev; // prev nodes can help
	t_cmd *cmd_data; // actually we may not need a struct with cmd, just the value might be enough because of the logic wrote in the execution file
	t_redir *redir_data;
	t_args *cmd_args;
	int	node_i;
	int	nodes_total;// every node will store this data so we know how many pipes we have
	struct s_node *next;
	//not sure yet but we have to find a way to handle -n flag
	bool	n_flag; // flag that tells echo not to append a new_line to the echoed args
	int		n_flag_n; // writes how many time -n flag to see if last arg should be -
} t_node;

// IMPORTANT: AN ABSTRACT SYNTAX TREE IS A SEQUENCE OF NODE LISTS THAT ARE LINKED THROUGH EACHOTHER WITH AN OPERATOR(&& or ||) or a DELIMITER (;)
// THEREFORE ONLY IN THE BONUS WE CREATE A REAL ABSTRACT SYNTAX TREE.
// WITHOUT THE BONUS, ANY OF OUR COMMANDS ARE GROUPED IN NODE COMMANDS IN A SINGLE NODE TABLE: so we do not need to return the ast struct written below.
/* typedef	struct s_ast // wrote this just to better clarify my node structure
{
	t_node			*curr_node_list; // the cmd table made of nodes and pipes
	t_operator		*operator_data; // the && and || operators and ; // the ; delimeter (contais the operator type and value and node to the next cmd table)
	t_node			*next_node_list; // the cmd table made of nodes and pipes
}	t_ast; // without bonus  t_ast will only return 1 cmd_line while the rest of the data is empty */


//Main function that will return the node list to the main
t_node	*return_nodelist(t_token_list *token_list);

/* ---- Supporting functions --- */

//Functions used to allocate and append nodes to nodelist
// t_node *create_node();
void append_node(t_node **head, t_node *new_node);

//Functions used to allocate and append cmd struct to its parent node
void add_cmd_to_node(t_node *node_list, t_node *curr_node, t_token_list *token);

//Functions used to allocate and append args to arglist and then to its parent node
t_args *create_newarg_data(t_node *node_list, t_token_list *token);
void append_newarg_to_cmdargs(t_args **cmd_args, t_args *new_arg);
void add_cmdargs_to_node(t_node *node_list, t_node *curr_node, t_args *head_arg);

//Functions used to allocate and append cmd struct to its parent node
t_redir *create_redir_data(t_token_list *token);
void append_redir_data(t_redir **head_redir, t_redir *curr_redir);

//Functions to free node allocation and its children data
void free_node_list(t_node *node_list);

/* ------------------------------ */


//My mock tocken_list creation functions
t_token_list			*create_mock_tokens(char *input);
void			free_mock_tokens(t_token_list *head);

// Printing and debugging functions (we comment or remove them from our code when ready to submit)
void print_nodes(t_node *head);

// exec test
void	exec_node(t_node	*node_list);

#endif



//Good resources that helped getting me into this and that will allow us to expand our logic:
//https://github.com/DimitriDaSilva/42_minishell/blob/master/src/parse/parse.c#L100
//https://github.com/mit-pdos/xv6-riscv/blob/riscv/user/sh.c
// brainstorming and asking questions to chat_gpt
// testing command lines in bash
// google & youtube


