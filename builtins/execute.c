#include "../headers/minishell.h"
#include "../headers/built_in.h"


int is_builtin(t_tree_node *node)
{
	if (node->type == N_EXEC && node->data.exec_u.args[0] != NULL)
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

int execute_builtin(t_tree_node *node, t_context *msh)
{
	bld_in *builtins;
	bld_in *builtin;

	builtins = create_builtin_list();
	builtin = find_builtin(builtins, node);

	if (builtin)
	{
		builtin->func(node, msh);
		msh->ret_exit = 0;
	}
	else
	{
		fprintf(stderr, "minishell: %s: command not found\n", node->data.exec_u.args[0]);
		msh->ret_exit = 127;
	}
	free_builtin_list(builtins);
	return (msh->ret_exit);
}

void pipe_builtin(t_tree_node *node, t_context *msh)
{
    int backup_fd[2]; // Backup stdin and stdout

    // Backup original stdin and stdout
    backup_fd[0] = dup(STDIN_FILENO);
    backup_fd[1] = dup(STDOUT_FILENO);

    // Redirect stdin and stdout if needed
    if (msh->fd[0] != STDIN_FILENO)
    {
        dup2(msh->fd[0], STDIN_FILENO);
        close(msh->fd[0]);
    }
    if (msh->fd[1] != STDOUT_FILENO)
    {
        dup2(msh->fd[1], STDOUT_FILENO);
        close(msh->fd[1]);
    }

    // Execute the built-in directly
    execute_builtin(node, msh);

    // Restore original stdin and stdout
    dup2(backup_fd[0], STDIN_FILENO);
    dup2(backup_fd[1], STDOUT_FILENO);
    close(backup_fd[0]);
    close(backup_fd[1]);
}
