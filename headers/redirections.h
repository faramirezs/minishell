#ifndef REDIRECTIONS_H
# define REDIRECTIONS_H

typedef struct s_redircmd t_redircmd;

struct s_redircmd
{
	struct s_redir *prev;
	int     redir_type;      // REDIR_IN, REDIR_OUT, APPEND_OUT, HEREDOC
	int     redir_i;         // Order of redirection
	char    *target;         // Filename or delimiter
	int     target_type;     // TARGET_FILENAME, TARGET_PATHNAME, TARGET_ENV_PATHNAME, TARGET_DELIMITER
	int     target_token_type;
	char	**args;
	// Execution data
	char    *exec_path;      // Path for execution
	char    *exec_file;      // Actual file to execute on
	char    *file_input;     // File content as single string
	char    **split_input;   // File content as lines

	// File handling
	int     flags;          // O_RDONLY, O_WRONLY, O_APPEND, etc.
	int     fd;            // File descriptor
	int    close_fd;      // Resource management flag
	int     error_code;    // For storing specific error states

	struct s_redir *next;
};

t_tree_node *parse_redir(t_scanner *scanner, t_args *cmd_args);

#endif
