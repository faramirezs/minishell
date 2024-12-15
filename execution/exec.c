#include "../headers/minishell.h"

#define FORKED_CHILD 0

static void exec_command(t_tree_node *node);

void exec(t_tree_node *node)
{
	printf("exec()\n");
	if (node->type == N_EXEC)
		exec_command(node);
	else
		printf("It is not a EXE_N node\n");
}

static void exec_command(t_tree_node *node)
{
	(void)node; //To silence unused parameter alert.
	printf("exec_command()\n");
	if (fork() == FORKED_CHILD)
	{
		printf("Fork 0");
		//char *argv[2] = { "ls", 0};
		//execvp(argv[0], argv);
		execvp(node->data.exec_u.cmd, node->data.exec_u.args);
	}
	wait(NULL);
}
