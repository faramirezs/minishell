#ifndef NODE_H
#define NODE_H

typedef enum e_node_type
{
	//N_ERR = -1, just an idea for now
	N_EXEC,
	N_REDIR,
	N_PIPE
	//N_LIST, //for `;` `&&` and `||`
	//N_BACK
} t_node_type;


//node selector, like pick one and put it on the node struct (saves memory).

typedef union
{
	t_execcmd exec_u;
	t_redircmd redir_u;
	t_pipecmd pipe_u;

} u_node_value;

// I think we do not need this one because we have the node struct.
typedef struct s_cmd
{
	t_node_type type;
} t_cmd;

typedef struct s_execcmd
{
	//t_node_type type;
	char *argv[MAXARGS];
	char *eargv[MAXARGS];
} t_execcmd;

typedef struct s_redircmd
{
	//t_node_type type;
	t_cmd *cmd;
	char *file;
	char *efile;
	int mode;
	int fd;
} t_redircmd;

typedef struct s_pipecmd
{
	//t_node_type type;
	//t_cmd *left;
	//t_cmd *right;
	t_node *left;
	t_node *right;

} t_pipecmd;

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

typedef struct s_node
{
	t_node_type u_type;
	u_node_value data;
} t_node;

//Constructor, each constructor is responsible for set the node type
t_node *exec_node_new(char **argv, char **eargv); //Not sure what should be the input here
t_node *redir_node_new(char *file, char *efile, int mode, int fd);
t_node *pipe_node_new(t_node *left, t_node *right);



//because we are allocating heap  memory for each constrcutor function
void node_drop(t_node *self); //To free up any memory that is allocated with the constructors

#endif
