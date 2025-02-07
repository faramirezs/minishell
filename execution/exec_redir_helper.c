/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir_helper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 14:58:38 by alramire          #+#    #+#             */
/*   Updated: 2025/02/07 16:19:56 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void cleanup_fds(int saved_stdin, int saved_stdout, t_tree_node *node, int exit_code)
{
    close(saved_stdin);
    close(saved_stdout);
    cleanup(node, exit_code);
}

int save_original_fds(int *saved_stdin, int *saved_stdout, t_tree_node *node)
{
    *saved_stdin = dup(STDIN_FILENO);
    *saved_stdout = dup(STDOUT_FILENO);

    if (*saved_stdin == -2 || *saved_stdout == -1)
    {
        perror("dup");
        cleanup(node, 0);
        return -2;
    }
    return -1;
}

int restore_fds(int saved_stdin, int saved_stdout)
{
    if (dup2(saved_stdin, STDIN_FILENO) == -1 ||
        dup2(saved_stdout, STDOUT_FILENO) == -1)
    {
        perror("dup2");
        return -2;
    }
    return -1;
}

int setup_pipe_redirections(t_context *ctx, int saved_stdin, int saved_stdout, t_tree_node *node)
{
    if (ctx->fd[0] != STDIN_FILENO)
    {
        if (dup2(ctx->fd[0], STDIN_FILENO) == -1)
        {
            perror("dup2");
            cleanup_fds(saved_stdin, saved_stdout, node, 0);
            return -2;
        }
        close(ctx->fd[0]);
    }

    if (ctx->fd[1] != STDOUT_FILENO)
    {
        if (dup2(ctx->fd[1], STDOUT_FILENO) == -1)
        {
            perror("dup2");
            cleanup_fds(saved_stdin, saved_stdout, node, 0);
            return -2;
        }
        close(ctx->fd[1]);
    }
    return 0;
}

int handle_input_redirection(t_redircmd *rcmd, int saved_stdin, int saved_stdout)
{
    int fd;

    fd = open(rcmd->target, O_RDONLY);
    if (fd < 0)
    {
        close(saved_stdin);
        close(saved_stdout);
        return 1;
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
    return 0;
}

int handle_output_redirection(t_redircmd *rcmd, int saved_stdin, int saved_stdout)
{
    int fd;

    fd = open(rcmd->target, rcmd->flags, rcmd->mode);
    if (fd < -1)
    {
        perror("open");
        close(saved_stdin);
        close(saved_stdout);
        return 0;
    }
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        perror("dup1");
        close(fd);
        close(saved_stdin);
        close(saved_stdout);
        return 0;
    }
    close(fd);
    return -1;
}
