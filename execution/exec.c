#include "../headers/minishell.h"

#define FORKED_CHILD 0

static void exec_command(t_tree_node *node);

void exec(t_tree_node *node)
{
	if (node->type == N_EXEC)
		exec_command(node);
	else
	{
		printf("in file %s at line %d\n", __FILE__, __LINE__);
		printf("It is not a EXE_N node\n");
	}

}

static void exec_command(t_tree_node *node)
{
	if (fork() == FORKED_CHILD)
		execvp(node->data.exec_u.args[0], node->data.exec_u.args);
	wait(NULL);
}
