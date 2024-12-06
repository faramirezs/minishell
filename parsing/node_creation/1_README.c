/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1_README.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 14:37:17 by jslusark          #+#    #+#             */
/*   Updated: 2024/11/26 12:22:36 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*

BETTER EXPLANATION OF STRUCTS ANS COMMENTS NEAR MY CODE:

The tokenization process (lexical analysis of input prompted on the shell), should return
a linked list of tockens which will then be used to create a double linked list of nodes.
I thought that a double linked list could easily help us in traversing the nodes when having to handle pipe execution.
Each node will be the parent of the following data:

- S_NODE *PREV
		Pointer to the previous node.

- S_NODE *NEXT
		Pointer to the next node.

- CMD_STRUCT
	Holds the type and value of the token that bash detects as command of the node.
	Bash detects as a command of the node any token that is not redirection data or pipe.
	I am unsure if we need a struct or just the token value is enough (based on what I was thinking about executing the commands).
	My thoughts come from writing this in bash:
		- "hello echo" it will give you error.
		- '$PATH', it will give it as error.
		- '$UNKNOWN' will not write anything and not give an error... unsure why
	Probably let's do other tests to see if we need to decide to change this.

- REDIRECTION STRUCT (t_redir *redir_data)
	The struct is a double linked list.
	A linked list because a node can have more than 1 redirection as children,
	so we need to have access to the next redirs if we have more than one.
	It's a double linked list because I think there might be cases where this could be useful but not sure yet.
	The redir struct contains this data:
		- redir_type: which is the type of redirection
		- redir_i: index of the redirection (this might be useful in our execution to understand if we are executing in the right order when having more than 1 redirs)
		- target: which is the name of the target where the redirection executes(a file, path to a file or delimiter).
		- target type: which is the type of the target. Input, output and append redir are always followed by files or path to files, heredoc are ONLY followed by a delimiter.
		- target_token_type: type of the token that becomes the redir's target, this info is helpful for understanding already if we have a path or file in the case we have an output, input or heredoc redirection.
		- s_redir *next: pointer to the next redirection (if we have just one redir, next is NULL)
		- s_redir *prev: pointer to the prev redirection (for the first/head redir struct, prev is NULL)

		When we are going to execute redirections, I think we should add this data to each redirection as well:
		- exec_path: name of the path where we have to execute the redirection, if target type is path_to_file exec_path will be the path of the target, if target_type is file the exec_path is the path where we are located (when starting the shell or navigating in directories via cd)
		- exec_file: the actual file which has "target" as file name. If the file does not exist we create it and name it as "target" (if target type is not word we have to get the last word in path (comes after last /). We then execute on this file, we read if it's from an output redir, or if it's input or append redir we have to check first if the file already has text that needs to be overwritten or appended to.
		- file_input: the data that is inside the file, we can store everything together in a string at the beginning
		- split_input: the file input split in different lines if we encounter a new line (\n)
		- flags: flags that are used wwith the open function, which we will use when executing redirections (each redirection type requires different flags)
		- int fd: file descriptor for error handling when we open the file and process its input
		- bool close_fd: Indicates if the fd should be closed after use. Undure if we actually need this for error handling

- ARGUMENT STRUCT (t_args *cmd_args)
	The struct is a double linked list.
	A linked list because a node can have more than 1 argument that need to be executed by its command,
	so we need to have access to the next arguments if we have more than one.
	It's a double linked list because I think there might be cases where this could be useful but not sure yet.
	The args struct contains this data:
		- arg_type:
		- arg_value
		- s_args *next: pointer to the next arg (if we have just one arg, next is NULL)
		- s_args *prev: pointer to the prev arg (for the first/head arg struct, prev is NULL)

- INDEX OF THE NODE (int node_i)
	index of the node, it could help us to make sure of the order of the node when executing pipes.

- TOTAL NUMBER OF NODES (int nodes_total)
	the total number of nodes in the list, it will help us understanding how many nodes we need to execute.
	Unsure if assignig this data to every node, have it as a global variable (probably better not) or just have it as a variable that then we pass as a parameter to functions that will need it.

- OPTION DATA (-n option used only with echo)
	Have a rough idea on how to handle it but it may be a bit different from what i was thinking below:
		- bool	n_flag: if in our token list we have a token_value "-n", n_flg is true and triggers our shell not to append a new_line when echoing args
		- int	n_flag_n: writes how many times -n flag was written in the input, if it appears more than once we have to echo the args preceded with -

*/