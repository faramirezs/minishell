/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 16:59:59 by alramire          #+#    #+#             */
/*   Updated: 2025/02/15 18:08:24 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

#define FORKED_CHILD 0

int	create_pipe(int pipefd[2])
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	return (0);
}

pid_t	fork_and_exec(t_tree_node *node, t_context *ctx, int close_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		close(close_fd);
		exit(exec_node(node, ctx));
	}
	return (pid);
}

int	wait_for_children(pid_t left_pid, pid_t right_pid)
{
	int	right_status;

	waitpid(left_pid, NULL, 0);
	waitpid(right_pid, &right_status, 0);
	if (WIFEXITED(right_status) && WEXITSTATUS(right_status) != 0)
	{
		return (WEXITSTATUS(right_status));
	}
	return (0);
}

int	exec_pipe(t_tree_node *node, t_context *ctx)
{
	int			pipefd[2];
	t_context	left_ctx;
	t_context	right_ctx;
	pid_t		left_pid;
	pid_t		right_pid;

	left_ctx = *ctx;
	right_ctx = *ctx;
	if (create_pipe(pipefd) == -1)
	{
		return (-1);
	}
	left_ctx.fd[1] = pipefd[1];
	right_ctx.fd[0] = pipefd[0];
	left_pid = fork_and_exec(node->data.pipe_u.left, &left_ctx, pipefd[0]);
	right_pid = fork_and_exec(node->data.pipe_u.right, &right_ctx, pipefd[1]);
	close(pipefd[0]);
	close(pipefd[1]);
	return (wait_for_children(left_pid, right_pid));
}
