#include <stdlib.h>

#include "node.h"
#include "guards.h"

//YouTube video min 7:45 https://youtu.be/sUxFE32tXF0?si=73UiqQEYAERD3fdD

t_node *exec_node_new(char **argv, char **eargv)
{
	t_node *node;

	//node = malloc(sizeof(t_node));
	node = OOM_GUARDS(malloc(sizeof(t_node)), __FILE__, __LINE__);
	//node->u_type = N_EXEC;
	//node->data.
}
t_node *redir_node_new(char *file, char *efile, int mode, int fd)
{

}

t_node *pipe_node_new(t_node *left, t_node *right)
{

}


// Video used to get until this point: https://www.youtube.com/watch?v=sUxFE32tXF0&t=995s&ab_channel=KrisJordan
