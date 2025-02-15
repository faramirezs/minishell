/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 16:59:59 by alramire          #+#    #+#             */
/*   Updated: 2025/02/15 18:00:51 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	cleanup(t_tree_node *node, int exit_code)
{
	free_tree_node(node);
	clear_history();
	exit(exit_code);
}

int	setup_pipe_redirection(t_context *ctx, int saved_stdin, int saved_stdout,
		t_tree_node *node)
{
	if (ctx->fd[0] != STDIN_FILENO)
	{
		if (dup2(ctx->fd[0], STDIN_FILENO) == -1)
		{
			perror("dup2");
			close(saved_stdin);
			close(saved_stdout);
			cleanup(node, 1);
		}
		close(ctx->fd[0]);
	}
	if (ctx->fd[1] != STDOUT_FILENO)
	{
		if (dup2(ctx->fd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2");
			fprintf(stderr, "setup_pipe\n");
			close(saved_stdin);
			close(saved_stdout);
			cleanup(node, 1);
		}
		close(ctx->fd[1]);
	}
	return (0);
}

int	save_std_fds(int *saved_stdin, int *saved_stdout, t_tree_node *node)
{
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	if (*saved_stdin == -1 || *saved_stdout == -1)
	{
		perror("dup");
		cleanup(node, 1);
		return (1);
	}
	return (0);
}

void	restore_std_fds(int saved_stdin, int saved_stdout, t_tree_node *node)
{
	if (dup2(saved_stdin, STDIN_FILENO) == -1)
	{
		perror("dup2");
		fprintf(stderr, "restore_std_fds1\n");
		close(saved_stdin);
		close(saved_stdout);
		cleanup(node, 1);
	}
	if (dup2(saved_stdout, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		fprintf(stderr, "restore_std_fds\n");
		close(saved_stdin);
		close(saved_stdout);
		cleanup(node, 1);
	}
	close(saved_stdin);
	close(saved_stdout);
}
