/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 16:59:59 by alramire          #+#    #+#             */
/*   Updated: 2025/02/15 18:03:59 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

#define FORKED_CHILD 0

static int	create_and_fork_heredoc(int pipefd[2], pid_t *pid)
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	*pid = fork();
	if (*pid == -1)
	{
		perror("fork");
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	return (0);
}

static void	write_heredoc_content(int pipefd, const char *content)
{
	ssize_t	bytes_written;
	size_t	content_len;

	if (content)
	{
		content_len = ft_strlen(content);
		bytes_written = write(pipefd, content, content_len);
		if (bytes_written == -1)
		{
			perror("write");
			exit(1);
		}
	}
}

static int	handle_heredoc_child(t_redircmd *rcmd)
{
	close(rcmd->heredoc_pipe[0]);
	write_heredoc_content(rcmd->heredoc_pipe[1], rcmd->heredoc_content);
	close(rcmd->heredoc_pipe[1]);
	exit(0);
}

static int	handle_heredoc_parent(t_redircmd *rcmd)
{
	int	status;

	close(rcmd->heredoc_pipe[1]);
	if (dup2(rcmd->heredoc_pipe[0], STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(rcmd->heredoc_pipe[0]);
		return (-1);
	}
	close(rcmd->heredoc_pipe[0]);
	waitpid(rcmd->heredoc_pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		return (-1);
	return (0);
}

int	handle_heredoc(t_redircmd *rcmd)
{
	if (create_and_fork_heredoc(rcmd->heredoc_pipe, &rcmd->heredoc_pid) == -1)
		return (-1);
	if (rcmd->heredoc_pid == FORKED_CHILD)
		handle_heredoc_child(rcmd);
	return (handle_heredoc_parent(rcmd));
}