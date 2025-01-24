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
/*
void execute_builtin(t_tree_node *node, t_context *msh)
{
	char **args;

	args = node->data.exec_u.args;

	if (strcmp(args[0], "cd") == 0)
		handle_cd(args, msh);
	else if (strcmp(args[0], "echo") == 0)
		handle_echo(args, msh);
	else if (strcmp(args[0], "pwd") == 0)
		handle_pwd(args, msh);
	else if (strcmp(args[0], "export") == 0)
		handle_export(args, msh);
	else if (strcmp(args[0], "unset") == 0)
		handle_unset(args, msh);
	else if (strcmp(args[0], "env") == 0)
		handle_env(args, msh);
	else if (strcmp(args[0], "exit") == 0)
		handle_exit(args, msh);
	else
	{
		// Error handling for unknown commands
		fprintf(stderr, "minishell: %s: command not found\n", args[0]);
		msh->ret_exit = 127; // Set exit code for "command not found"
	}
}
*/