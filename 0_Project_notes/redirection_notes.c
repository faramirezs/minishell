/*The redir struct contains this data:
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

*/
