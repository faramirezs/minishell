#include "../headers/minishell.h"

#define FORKED_CHILD 0


/* Exit Codes and meaning (we should remember to assign them after parsing and executing)
	0 	- Success: Command executed successfully.
	1 	- General error: Command failed for a generic reason.
	2 	- Incorrect usage: Invalid arguments or syntax in the command (Jess can add this in main.c - if(!node_list) after the node is freed)
	126	- Cannot execute: File exists but is not executable.
	127	- Command not found: Command is missing in the system's PATH.
	130	- Script interrupted (SIGINT): Process terminated via Ctrl+C. */

//From this video https://www.youtube.com/watch?v=KbhDPYHRqkY&list=PLKUb7MEve0TjHQSKUWChAWyJPCpYMRovO&index=67
// Execute pipe node https://www.youtube.com/watch?v=KbhDPYHRqkY&list=PLKUb7MEve0TjHQSKUWChAWyJPCpYMRovO&index=65

typedef struct s_context
{
	int fd[2]; //for stdin and stdout
	int fd_close; // Close an fd? -1 if not
} t_context;

static int exec_node(t_tree_node *node, t_context *ctx);
static int exec_command(t_tree_node *node, t_context *ctx);
static int exec_pipe(t_tree_node *node, t_context *ctx);

// Executing command https://www.youtube.com/watch?v=HzAQCUB9Ifw&list=PLKUb7MEve0TjHQSKUWChAWyJPCpYMRovO&index=63&t=826s
//

static int exec_node(t_tree_node *node, t_context *ctx)
{
	if (node->type == N_EXEC)
		return (exec_command(node, ctx));
	else if (node->type == N_PIPE)
		return (exec_pipe(node, ctx));
	else
	{
		printf("in file %s at line %d\n", __FILE__, __LINE__);
		printf("It is not a EXE_N node\n");
		return (0);
	}
}


void exec(t_tree_node *node)
{
	t_context ctx;
	int children;
	int i;

	i = 0;
	children = 0;
	ctx.fd[0] = STDIN_FILENO;
	ctx.fd[1] = STDOUT_FILENO;
	ctx.fd_close = -1;
	children = exec_node(node, &ctx);
	while(i < children)
	{
		printf("Value of i: %i, wait()\n", i);
		wait(NULL);
		i++;
	}
}

static int exec_command(t_tree_node *node, t_context *ctx)
{
	pid_t pid;

	pid = fork();
	sleep(500);
	if (pid == FORKED_CHILD)
	{
		printf("Child pID: %d\n", getpid());
		//evaluate the context and act on
		dup2(ctx->fd[STDIN_FILENO], STDIN_FILENO);
		dup2(ctx->fd[STDOUT_FILENO], STDOUT_FILENO);
		if(ctx->fd_close >= 0)
			close(ctx->fd_close);
		//printf("+++Exec_command()check+++\n");
		//check_null_array(node->data.exec_u.args);
		execvp(node->data.exec_u.args[0], node->data.exec_u.args);
	}
	else if (pid > 0)
	{
		printf("Parent pID: %d\n", getpid());
	}
	return (1);
}

static int exec_pipe(t_tree_node *node, t_context *ctx)
{
	t_tree_node *lhs;
	t_tree_node *rhs;
	t_context lhs_ctx;
	t_context rhs_ctx;
	int p[2];
	int children;

	children = 0;
	pipe(p);
	lhs = node->data.pipe_u.left;
	lhs_ctx = *ctx;
	lhs_ctx.fd[STDOUT_FILENO] = p[STDOUT_FILENO];
	lhs_ctx.fd_close = p[STDIN_FILENO];
	children += exec_node(lhs, &lhs_ctx);
	rhs = node->data.pipe_u.right;
	rhs_ctx = *ctx;
	rhs_ctx.fd[STDIN_FILENO] = p[STDIN_FILENO];
	rhs_ctx.fd_close = p[STDOUT_FILENO];
	children += exec_node(rhs, &rhs_ctx);
	close(p[STDIN_FILENO]);
	close(p[STDOUT_FILENO]);
	return (children);
}
