#ifndef TREE_NODE_H
#define TREE_NODE_H

#include "../headers/scanner.h"
#include "../headers/args.h"

typedef struct s_cmd t_cmd;
typedef struct s_execcmd t_execcmd;
typedef struct s_redircmd t_redircmd;
typedef struct s_pipecmd t_pipecmd;
typedef union u_node_value u_node_value;
typedef struct s_tree_node t_tree_node;

//node selector, like pick one and put it on the node struct (saves memory).
typedef enum e_node_type
{
	//N_ERR = -1, just an idea for now
	N_EXEC,
	N_REDIR,
	N_PIPE
	//N_LIST, //for `;` `&&` and `||`
	//N_BACK
} t_node_type;


// I think we do not need this one because we have the node struct.
struct s_cmd
{
	t_node_type type;
};

struct s_execcmd
{
	//t_node_type type;
	//char *argv[MAXARGS];
	//char *eargv[MAXARGS];
	//char *cmd;
	char **args;
};

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

struct s_pipecmd
{
	//t_node_type type;
	//t_cmd *left;
	//t_cmd *right;
	t_tree_node *left;
	t_tree_node *right;

};

union u_node_value
{
	t_execcmd exec_u;
	t_redircmd redir_u;
	t_pipecmd pipe_u;
};

struct s_tree_node
{
	t_node_type type;
	u_node_value data;
};



/* typedef struct s_listcmd
{
	t_node_type type;
	t_cmd *left;
	t_cmd *right;
} t_listcmd;

typedef struct s_backcmd
{
	t_node_type type;
	t_cmd *cmd;
} t_backcmd; */




//Constructor, each constructor is responsible for set the node type
t_tree_node *exec_node_new(char *cmd, char *args); //Not sure what should be the input here
t_tree_node *redir_node_new(char *file, char *efile, int mode, int fd);
t_tree_node *pipe_node_new(t_tree_node *left, t_tree_node *right);



//because we are allocating heap  memory for each constrcutor function
void node_drop(t_tree_node *self); //To free up any memory that is allocated with the constructors

t_tree_node *parse_tree_node (t_scanner *scanner);
t_tree_node *parse_exec(t_args *args);
t_tree_node *parse_redir (t_scanner *scanner, t_args *args);
// t_tree_node *parse_redir ();
t_tree_node *parse_pipe (t_scanner *scanner, t_args *args);
//t_tree_node *error_node(const char *msg);
void indent_node (size_t spaces);
void visit_node (const t_tree_node *node, size_t spaces);


#endif
