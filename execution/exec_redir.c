/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 16:59:59 by alramire          #+#    #+#             */
/*   Updated: 2025/02/14 17:52:05 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

#define FORKED_CHILD 0

int	setup_pipe_redirection(t_context *ctx, int saved_stdin,
	int saved_stdout, t_tree_node *node)
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

int	redirect_input(t_redircmd *rcmd, int saved_stdin, int saved_stdout,
	t_tree_node *node)
{
int	fd;

(void)saved_stdin;
(void)saved_stdout;
(void)node;
fd = open(rcmd->target, O_RDONLY);
if (fd < 0)
{
	perror(rcmd->target);
	return (1);
}
if (dup2(fd, STDIN_FILENO) == -1)
{
	perror("dup2");
	fprintf(stderr, "redirect_input\n");
	close(fd);
	return (1);
}
close(fd);
return (0);
}

int	redirect_output(t_redircmd *rcmd, int saved_stdin, int saved_stdout,
	t_tree_node *node)
{
	int	fd;

	(void)saved_stdin;
	(void)saved_stdout;
	(void)node;
	fd = open(rcmd->target, rcmd->flags, rcmd->mode);
	if (fd < 0)
	{
		perror(rcmd->target);
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		fprintf(stderr, "redirect_output\n");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	apply_redirection(t_redircmd *rcmd, int saved_stdin,
	int saved_stdout, t_tree_node *node)
{
if (rcmd->redir_type == REDIR_IN)
	return (redirect_input(rcmd, saved_stdin, saved_stdout, node));
else if (rcmd->redir_type == HEREDOC)
{
	if (handle_heredoc(rcmd) < 0)
	{
		close(saved_stdin);
		close(saved_stdout);
		cleanup(node, EXIT_FAILURE);
	}
}
else if (rcmd->redir_type == REDIR_OUT || rcmd->redir_type == APPEND_OUT)
	return (redirect_output(rcmd, saved_stdin, saved_stdout, node));
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

void	restore_std_fds(int saved_stdin, int saved_stdout,
	t_tree_node *node)
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
int	exec_redir(t_tree_node *node, t_context *ctx)
{
t_redircmd	*rcmd;
int			saved_stdin;
int			saved_stdout;
int			result;

rcmd = &node->data.redir_u;
result = 0;
if (save_std_fds(&saved_stdin, &saved_stdout, node) != 0)
	return (1);
if (setup_pipe_redirection(ctx, saved_stdin, saved_stdout, node) != 0)
{
	restore_std_fds(saved_stdin, saved_stdout, node);
	return (1);
}
if (apply_redirection(rcmd, saved_stdin, saved_stdout, node) != 0)
{
	restore_std_fds(saved_stdin, saved_stdout, node);
	return (1);
}
if (rcmd->cmd)
	result = exec_node(rcmd->cmd, ctx);
return (result);
}
