/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 16:59:59 by alramire          #+#    #+#             */
/*   Updated: 2025/03/07 21:41:59 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

#define FORKED_CHILD 0

int	exec_node(t_tree_node *node, t_context *ctx)
{
	if (node->type == N_EXEC)
		return (exec_command(node, ctx));
	else if (node->type == N_PIPE)
		return (exec_pipe(node, ctx));
	else if (node->type == N_REDIR)
		return (exec_redir(node, ctx));
	else
	{
		printf("in file %s at line %d\n", __FILE__, __LINE__);
		printf("It is not a EXEC_N node\n");
		return (1);
	}
}

int	exec(t_tree_node *node, t_context *msh)
{
	int	children;
	int	status;

	if(node == NULL)
		return(1);
	msh->fd[0] = STDIN_FILENO;
	msh->fd[1] = STDOUT_FILENO;
	msh->fd_close = -1;
	msh->ret_exit = exec_node(node, msh);
	children = msh->ret_exit;
	while (children > 0)
	{
		wait(&status);
		children--;
	}
	return (msh->ret_exit);
}

int	exec_builtin_command(t_tree_node *node, t_context *ctx)
{
	if (ctx->fd[0] != STDIN_FILENO)
	{
		dup2(ctx->fd[0], STDIN_FILENO);
		close(ctx->fd[0]);
	}
	if (ctx->fd[1] != STDOUT_FILENO)
	{
		dup2(ctx->fd[1], STDOUT_FILENO);
		close(ctx->fd[1]);
	}
	return (execute_builtin(node, ctx));
}

/* static void	exec_minishell(t_tree_node *node, t_context *ctx)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (path)
	{
		path = ft_strjoin_free_s1(path, "/minishell");
		execve(path, node->data.exec_u.args, ctx->env);
		free(path);
	}
} */

void	exec_child_process(t_tree_node *node, t_context *ctx)
{
	if (ctx->fd[0] != STDIN_FILENO)
	{
		dup2(ctx->fd[0], STDIN_FILENO);
		close(ctx->fd[0]);
	}
	if (ctx->fd[1] != STDOUT_FILENO)
	{
		dup2(ctx->fd[1], STDOUT_FILENO);
		close(ctx->fd[1]);
	}
	execvp(node->data.exec_u.args[0], node->data.exec_u.args);
	perror("execvp");
	free_tree_node(&node);
	free_builtin_list(&ctx->builtins);
	cleanup_context(ctx);
	exit(127);
}

/* void	exec_child_process(t_tree_node *node, t_context *ctx)
{
	if (ctx->fd[0] != STDIN_FILENO)
	{
		dup2(ctx->fd[0], STDIN_FILENO);
		close(ctx->fd[0]);
	}
	if (ctx->fd[1] != STDOUT_FILENO)
	{
		dup2(ctx->fd[1], STDOUT_FILENO);
		close(ctx->fd[1]);
	}
	if (ft_strcmp(node->data.exec_u.args[0], "./minishell") == 0)
		exec_minishell(node, ctx);
	// else
	// 	execvp(node->data.exec_u.args[0], node->data.exec_u.args);
	else if (node->data.exec_u.args[0][0] == '/' || 
		node->data.exec_u.args[0][0] == '.')
	{
		execve(node->data.exec_u.args[0], node->data.exec_u.args, ctx->env);
	}
	else
	{
		if (!ms_get_env(ctx->env, "PATH"))
		{
			fprintf(stderr, "%s: No such file or directory\n", 
			   node->data.exec_u.args[0]);
			free_tree_node(&ctx->origin_node);
			free_builtin_list(&ctx->builtins);
			cleanup_context(ctx);
			exit(127);
		}
	{
		execvp(node->data.exec_u.args[0], node->data.exec_u.args);
		free_tree_node(&ctx->origin_node);
		free_builtin_list(&ctx->builtins);
		cleanup_context(ctx);
	}
	}
	perror("execvp");
	free_tree_node(&ctx->origin_node);
	free_builtin_list(&ctx->builtins);
	cleanup_context(ctx);
	exit(127);
} */

int	exec_command(t_tree_node *node, t_context *ctx)
{
	pid_t	pid;
	int		status;

	if (is_builtin(node))
		return (exec_builtin_command(node, ctx));
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}
	if (pid == FORKED_CHILD)
		exec_child_process(node, ctx);
	if (ctx->fd[0] != STDIN_FILENO)
		close(ctx->fd[0]);
	if (ctx->fd[1] != STDOUT_FILENO)
		close(ctx->fd[1]);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		return (WEXITSTATUS(status));
	return (0);
}
