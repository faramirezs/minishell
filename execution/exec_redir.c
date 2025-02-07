/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 14:56:56 by alramire          #+#    #+#             */
/*   Updated: 2025/02/07 16:24:30 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

int handle_echo_redirection(t_redircmd *rcmd, t_context *ctx)
{
    if (rcmd->cmd && rcmd->cmd->type == N_EXEC &&
        ft_strcmp(rcmd->cmd->data.exec_u.args[0], "echo") == 0)
    {
        if (rcmd->redir_type == REDIR_IN)
            return exec_node(rcmd->cmd, ctx);
    }
    return -1;  // Not an echo command
}

/* int execute_redirection(t_redircmd *rcmd, t_context *ctx, int saved_stdin, int saved_stdout)
{
    int result;

    if (rcmd->redir_type == REDIR_IN)
    {
        if (handle_input_redirection(rcmd) != 0)
        {
            fprintf(stderr, "minishell: %s: No such file or directory\n", rcmd->target);
            return 1;
        }
    }
    else if (rcmd->redir_type == HEREDOC)
    {
        if (handle_heredoc(rcmd) < 0)
        {
            cleanup_fds(saved_stdin, saved_stdout, rcmd->cmd, EXIT_FAILURE);
            return EXIT_FAILURE;
        }
    }
    else if (rcmd->redir_type == REDIR_OUT || rcmd->redir_type == APPEND_OUT)
    {
        if (handle_output_redirection(rcmd, saved_stdin, saved_stdout) != 0)
            return 1;
    }

    if (rcmd->cmd == NULL)
    {
        fprintf(stderr, "Error: Command node is NULL\n");
        cleanup_fds(saved_stdin, saved_stdout, rcmd->cmd, 1);
        return 1;
    }

    result = exec_node(rcmd->cmd, ctx);
    return result;
} */

int exec_redir(t_tree_node *node, t_context *ctx)
{
    t_redircmd *rcmd;
    int saved_stdin;
    int saved_stdout;
    int result;

    rcmd = &node->data.redir_u;
    if (save_original_fds(&saved_stdin, &saved_stdout, node) < 0)
        return 1;

    if (setup_pipe_redirections(ctx, saved_stdin, saved_stdout, node) < 0)
        return 1;

   if (rcmd->cmd && rcmd->cmd->type == N_EXEC &&
        ft_strcmp(rcmd->cmd->data.exec_u.args[0], "echo") == 0)
    {
        if (rcmd->redir_type == REDIR_IN)
        {
            return exec_node(rcmd->cmd, ctx);
            goto cleanup;
        }
    }
    // Handle different redirection types
    if (rcmd->redir_type == REDIR_IN)
    {
        if (handle_input_redirection(rcmd, saved_stdin, saved_stdout) != 0)
            return 1;
    }
	/* else if (rcmd->redir_type == HEREDOC)
    {
        if (handle_heredoc(rcmd) < 0)
        {
            close(saved_stdin);
            close(saved_stdout);
            cleanup(node, EXIT_FAILURE);
        }
        result = exec_command(node, ctx);
        if (dup2(saved_stdin, STDIN_FILENO) == -1)
        {
            perror("dup2");
            close(saved_stdin);
            close(saved_stdout);
            cleanup(node, EXIT_FAILURE);
        }
        close(saved_stdin);
        close(saved_stdout);
        return result;
	} */
    else if (rcmd->redir_type == REDIR_OUT || rcmd->redir_type == APPEND_OUT)
    {
        if (handle_output_redirection(rcmd, saved_stdin, saved_stdout) != 0)
            return 1;
    }

    if (rcmd->cmd == NULL)
    {
        fprintf(stderr, "Error: Command node is NULL\n");
        cleanup_fds(saved_stdin, saved_stdout, node, 1);
        return 1;
    }

    result = exec_node(rcmd->cmd, ctx);

cleanup:
    if (restore_fds(saved_stdin, saved_stdout) < 0)
        cleanup_fds(saved_stdin, saved_stdout, node, 1);

    close(saved_stdin);
    close(saved_stdout);
    return result;
}


/* int exec_redir(t_tree_node *node, t_context *ctx)
{
    t_redircmd *rcmd;
    int saved_stdin;
    int saved_stdout;
    int result;
    int echo_result;

    rcmd = &node->data.redir_u;

    // Save and set up file descriptors
    if (save_original_fds(&saved_stdin, &saved_stdout, node) < 0)
        return 1;

    if (setup_pipe_redirections(ctx, saved_stdin, saved_stdout, node) < 0)
        return 1;

    // Check for echo special case
    echo_result = handle_echo_redirection(rcmd, ctx);
    if (echo_result >= 0)
    {
        result = echo_result;
        goto cleanup;
    }

    // Handle input/output redirections
    if (rcmd->redir_type == REDIR_IN)
    {
        if (handle_input_redirection(rcmd) != 0)
            return 1;
    }
    else if (rcmd->redir_type == HEREDOC)
    {
        if (handle_heredoc(rcmd) < 0)
        {
            cleanup_fds(saved_stdin, saved_stdout, node, EXIT_FAILURE);
            return EXIT_FAILURE;
        }
    }
    else if (rcmd->redir_type == REDIR_OUT || rcmd->redir_type == APPEND_OUT)
    {
        if (handle_output_redirection(rcmd, saved_stdin, saved_stdout) != 0)
            return 1;
    }

    if (rcmd->cmd == NULL)
    {
        fprintf(stderr, "Error: Command node is NULL\n");
        cleanup_fds(saved_stdin, saved_stdout, node, 1);
        return 1;
    }

    // Execute the command
    result = exec_node(rcmd->cmd, ctx);

cleanup:
    if (restore_fds(saved_stdin, saved_stdout) < 0)
        cleanup_fds(saved_stdin, saved_stdout, node, 1);

    close(saved_stdin);
    close(saved_stdout);
    return result;
} */

/* int exec_redir(t_tree_node *node, t_context *ctx)
{
    t_redircmd *rcmd;
    int saved_stdin;
    int saved_stdout;
    int result;
    int echo_result;

    rcmd = &node->data.redir_u;

    // Save and set up file descriptors
    if (save_original_fds(&saved_stdin, &saved_stdout, node) < 0)
        return 1;

    if (setup_pipe_redirections(ctx, saved_stdin, saved_stdout, node) < 0)
        return 1;

    // Check for echo special case
    echo_result = handle_echo_redirection(rcmd, ctx);
    if (echo_result >= 0)
    {
        result = echo_result;
        goto cleanup;
    }
	if (rcmd->redir_type == REDIR_IN)
    {
        if (handle_input_redirection(rcmd) != 0)
            return 1;
    }
    else if (rcmd->redir_type == REDIR_OUT || rcmd->redir_type == APPEND_OUT)
    {
        if (handle_output_redirection(rcmd, saved_stdin, saved_stdout) != 0)
            return 1;
    }

    if (rcmd->cmd == NULL)
    {
        fprintf(stderr, "Error: Command node is NULL\n");
        cleanup_fds(saved_stdin, saved_stdout, node, 1);
        return 1;
    }

    // Execute the redirection
    //result = execute_redirection(rcmd, ctx, saved_stdin, saved_stdout);
	result = exec_node(rcmd->cmd, ctx);

cleanup:
    if (restore_fds(saved_stdin, saved_stdout) < 0)
        cleanup_fds(saved_stdin, saved_stdout, node, 1);

    close(saved_stdin);
    close(saved_stdout);
    return result;
} */
