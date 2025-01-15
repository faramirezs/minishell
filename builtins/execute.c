#include "../headers/structs.h"
#include "../headers/built_in.h"



int is_builtin(struct t_tree_node *node)
{
    if (node->type == N_EXEC && node->data.exec_u.args[0] != NULL) //I have to dig more to check up what's wrong here :(
    {
        const char *cmd = node->data.exec_u.args[0];
        if (strcmp(cmd, "cd") == 0 || strcmp(cmd, "echo") == 0 ||
            strcmp(cmd, "pwd") == 0 || strcmp(cmd, "export") == 0 ||
            strcmp(cmd, "unset") == 0 || strcmp(cmd, "env") == 0 ||
            strcmp(cmd, "exit") == 0)
            return (1);
    }
    return (0);
}

void execute_builtin(struct t_tree_node *node)
{
    char **args;
	
	args = node->data.exec_u.args;

    if (strcmp(args[0], "cd") == 0)
        handle_cd(args);
    else if (strcmp(args[0], "echo") == 0)
        handle_echo(args);
    else if (strcmp(args[0], "pwd") == 0)
        handle_pwd(args);
    else if (strcmp(args[0], "export") == 0)
        handle_export(args);
    else if (strcmp(args[0], "unset") == 0)
        handle_unset(args);
    else if (strcmp(args[0], "env") == 0)
        handle_env(args);
    else if (strcmp(args[0], "exit") == 0)
        handle_exit(args);
}