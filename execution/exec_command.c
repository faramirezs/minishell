/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 16:59:59 by alramire          #+#    #+#             */
/*   Updated: 2025/02/14 18:07:26 by alramire         ###   ########.fr       */
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
	exit(127);
}

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
