#include "../headers/minishell.h"

#define FORKED_CHILD 0

//From this video https://www.youtube.com/watch?v=KbhDPYHRqkY&list=PLKUb7MEve0TjHQSKUWChAWyJPCpYMRovO&index=67

typedef struct s_context
{
	int fd[2]; //for stdin and stdout
	int fd_close; // Close an fd? -1 if not
} t_context;

static void exec_node(t_tree_node *node, t_context *ctx);
static void exec_command(t_tree_node *node, t_context *ctx);
static void exec_pipe(t_tree_node *node, t_context *ctx);

static void exec_node(t_tree_node *node, t_context *ctx)
{
	if (node->type == N_EXEC)
		exec_command(node, ctx);
	else if (node->type == N_PIPE)
		exec_pipe(node, ctx);
	else
	{
		printf("in file %s at line %d\n", __FILE__, __LINE__);
		printf("It is not a EXE_N node\n");
	}
}


void exec(t_tree_node *node)
{


}

static void exec_command(t_tree_node *node, t_context *ctx)
{
	if (fork() == FORKED_CHILD)
		execvp(node->data.exec_u.args[0], node->data.exec_u.args);
	wait(NULL);
}

static void exec_pipe(t_tree_node *node, t_context *ctx)
{
	t_tree_node *lhs;
	t_tree_node *rhs;

	int p[2];
	pipe(p)

	lhs = node->data.pipe_u.left;
	exec(lhs);
	rhs = node->data.pipe_u.right;
	exec(rhs);
}
