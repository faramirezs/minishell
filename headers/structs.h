#ifndef STRUCTS_H
# define STRUCTS_H

typedef struct s_redircmd	t_redircmd;
typedef struct s_execcmd	t_execcmd;
typedef struct s_pipecmd	t_pipecmd;
typedef union u_node_value	t_node_value;
typedef struct s_tree_node	t_tree_node;

typedef struct s_chart_itr
{
	const char	*cursor;
	const char	*sentinel1;
}				t_char_itr;

typedef struct s_slice
{
	const char		*start;
	size_t			length;
}					t_slice;

typedef enum e_token_type // good to have an order like below
{
	COMMAND,
	ENV_VAR,
	ABS_PATH,
	REL_PATH,
	REDIR_IN,
	REDIR_OUT,
	APPEND_OUT,
	HEREDOC,
	PIPE,
	STRING_D_QUOTES,
	STRING_S_QUOTES,
	WORD,
	UNKNOWN,
	END,
}					t_token_type;

typedef struct s_token
{
	t_token_type	type;
	t_slice			lexeme;
}					t_token;

typedef struct s_context
{
	int fd[2]; //for stdin and stdout
	int fd_close; // Close an fd? -1 if not
	char	**env;		   // Environment variables
	char	**env_export;	// Exported variables
	int	 ret_exit;		// Last command's exit code
	char	*user;		   // Current user's name or shell session info
} t_context;

typedef struct s_scanner
{
	t_char_itr	char_itr;
	char		char_next;
	t_token		next;
	t_context	*msh;
}				t_scanner;

typedef struct s_args
{
	int *count;
	char **words;
} t_args;

typedef enum e_node_type
{
	// N_ERR = -1, just an idea for now
	N_EXEC,
	N_REDIR,
	N_PIPE
	// N_LIST, //for `;` `&&` and `||`
	// N_BACK
}							t_node_type;

typedef enum e_target_type
{
	TARGET_FILENAME,
	TARGET_PATHNAME,
	TARGET_ENV_PATHNAME,
	TARGET_DELIMITER
}							t_target_type;

struct s_redircmd
{
	t_token_type redir_type;	 // Current redirection type (REDIR_IN, REDIR_OUT, etc.)
	char *target;				// Target file/delimiter
	t_target_type target_type;   // Type of target (filename, pathname, env var, delimiter)
	int source_fd;			   // Source file descriptor (usually 0 for input, 1 for output)
	int target_fd;			   // Target file descriptor
	int flags;				   // Open flags for the file
	mode_t mode;				 // File permissions when creating new files
	t_tree_node *cmd;		   // Command to be redirected
	int error_code;			 // Error tracking
	char *heredoc_content;	// Store the collected heredoc content
	int heredoc_pipe[2];	  // Pipe for heredoc data transfer
	pid_t heredoc_pid;		// Process ID for heredoc handling
};

struct						s_execcmd
{
	char					**args;
};

struct						s_pipecmd
{
	t_tree_node				*left;
	t_tree_node				*right;
};

union						u_node_value
{
	t_execcmd				exec_u;
	t_redircmd				redir_u;
	t_pipecmd				pipe_u;
};

struct						s_tree_node
{
	t_node_type				type;
	t_node_value			data;
};


#endif