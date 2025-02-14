/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 16:59:59 by alramire          #+#    #+#             */
/*   Updated: 2025/02/14 17:58:49 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

#define FORKED_CHILD 0


int	exec_pipe(t_tree_node *node, t_context *ctx)
{
	t_pipecmd	*pcmd;
	int			pipefd[2];
	t_context	left_ctx;
	t_context	right_ctx;
	int			right_status;
	pid_t		left_pid;
	pid_t		right_pid;

	pcmd = &node->data.pipe_u;
	left_ctx = *ctx;
	right_ctx = *ctx;
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return -1;
	}
	left_ctx.fd[1] = pipefd[1];
	right_ctx.fd[0] = pipefd[0];
	left_pid = fork();
	if (left_pid == 0)
	{
		close(pipefd[0]);
		exit(exec_node(pcmd->left, &left_ctx));
	}
	right_pid = fork();
	if (right_pid == 0)
	{
		close(pipefd[1]);
		exit(exec_node(pcmd->right, &right_ctx));
	}
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(left_pid, NULL, 0);
	waitpid(right_pid, &right_status, 0);
	if (WIFEXITED(right_status) && WEXITSTATUS(right_status) != 0)
	{
		return WEXITSTATUS(right_status);
	}
	return 0;
}
