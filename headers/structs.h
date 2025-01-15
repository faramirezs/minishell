#ifndef STRUCTS_H
# define STRUCTS_H

# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1

typedef struct s_msh
{
    char    **env;           // Environment variables
    char    **env_export;    // Exported variables
    int     ret_exit;        // Last command's exit code
    int     signal_flag;     // Signal handling flag (e.g., for SIGINT, SIGQUIT)
    char    *user;           // Current user's name or shell session info
} t_msh;

typedef struct s_redir
{
	int redir_type;  // Type of redirection
	char *target;    // Target file or delimiter
	int flags;       // Flags for open()
	int fd;          // File descriptor
	struct s_redir *next; // Next redirection
} t_redir;

typedef struct s_args
{
	int arg_type;      // Argument type
	char *arg_value;   // Argument value
	struct s_args *next; // Next argument in list
} t_args;

typedef struct s_node
{
	struct s_node *prev;    // Previous node
	char *cmd_value;        // Command value (e.g., "echo")
	t_redir *redir_data;    // Linked list of redirections
	t_args *cmd_args;       // Linked list of arguments
	struct s_node *next;    // Next node
} t_node;

typedef enum e_token_type
{
	COMMAND,
	ENV_VAR,
	ABS_PATH,
	REL_PATH,
	OPTION,
	REDIR_IN,
	REDIR_OUT,
	APPEND_OUT,
	HEREDOC,
	PIPE,
	STRING_D_QUOTES,
	STRING_S_QUOTES,
	WORD,
	UNKNOWN,
	END
} t_token_type;

typedef struct s_token
{
	t_token_type type; // Token type
	char *value;       // Token value
} t_token;

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

struct						s_redircmd
{
	struct s_redir			*prev;
	int						redir_type;
	int						redir_i;
	char					*target;
	int						target_type;
	int						target_token_type;
	char					**args;
	char					*exec_file;
	char					*file_input;
	char					**split_input;
	int						flags;
	int						fd;
	int						close_fd;
	int						error_code;

	struct s_redir			*next;
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
