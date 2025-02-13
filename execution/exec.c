#include "../headers/minishell.h"

#define FORKED_CHILD 0

static int exec_node(t_tree_node *node, t_context *ctx);
static int exec_command(t_tree_node *node, t_context *ctx);
static int exec_pipe(t_tree_node *node, t_context *ctx);
static int exec_redir(t_tree_node *node, t_context *ctx);

static int setup_pipe_redirection(t_context *ctx, int saved_stdin, int saved_stdout, t_tree_node *node)
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
	return 0;
}

static int redirect_input(t_redircmd *rcmd, int saved_stdin, int saved_stdout, t_tree_node *node)
{
	(void)saved_stdin;
	(void)saved_stdout;
	(void)node;
	int fd = open(rcmd->target, O_RDONLY);
	if (fd < 0)
	{
		perror(rcmd->target);
		return 1;
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		fprintf(stderr, "redirect_input\n");
		close(fd);
		return 1;
	}
	close(fd);
	return 0;
}

static int redirect_output(t_redircmd *rcmd, int saved_stdin, int saved_stdout, t_tree_node *node)
{
	(void)saved_stdin;
	(void)saved_stdout;
	(void)node;
	int fd = open(rcmd->target, rcmd->flags, rcmd->mode);
	if (fd < 0)
	{
		perror(rcmd->target);
		return 1;
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		fprintf(stderr, "redirect_output\n");
		close(fd);
		return 1;
	}
	close(fd);
	return 0;
}

static int apply_redirection(t_redircmd *rcmd, int saved_stdin, int saved_stdout, t_tree_node *node)
{
	if (rcmd->redir_type == REDIR_IN)
		return redirect_input(rcmd, saved_stdin, saved_stdout, node);
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
		return redirect_output(rcmd, saved_stdin, saved_stdout, node);
	return 0;
}

static int save_std_fds(int *saved_stdin, int *saved_stdout, t_tree_node *node)
{
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	if (*saved_stdin == -1 || *saved_stdout == -1)
	{
		perror("dup");
		cleanup(node, 1);
		return 1;
	}
	return 0;
}

static void restore_std_fds(int saved_stdin, int saved_stdout, t_tree_node *node)
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
static int exec_redir(t_tree_node *node, t_context *ctx)
{
    t_redircmd *rcmd = &node->data.redir_u;
    int saved_stdin, saved_stdout;
    int result = 0;

    if (save_std_fds(&saved_stdin, &saved_stdout, node) != 0)
        return 1;

    if (setup_pipe_redirection(ctx, saved_stdin, saved_stdout, node) != 0)
    {
        restore_std_fds(saved_stdin, saved_stdout, node);
        return 1;
    }
    if (apply_redirection(rcmd, saved_stdin, saved_stdout, node) != 0)
    {
        restore_std_fds(saved_stdin, saved_stdout, node);
        return 1;
    }
    if (rcmd->cmd)
        result = exec_node(rcmd->cmd, ctx);
    return result;
}



static int exec_node(t_tree_node *node, t_context *ctx)
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
		printf("It is not a EXE_N node\n");
		return (0);
	}
}

int exec(t_tree_node *node, t_context *msh)
{
	int children;
	int status;

	msh->fd[0] = STDIN_FILENO;
	msh->fd[1] = STDOUT_FILENO;
	msh->fd_close = -1;

	msh->ret_exit = exec_node(node, msh);
	children = msh->ret_exit;

	while (children > 0) {
		wait(&status);
		children--;
	}
	return (msh->ret_exit);
}

static int exec_command(t_tree_node *node, t_context *ctx)
{
	pid_t pid;
	int status;

	if (is_builtin(node))
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

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return -1;
	}
	if (pid == FORKED_CHILD)
	{
		// Set up pipe redirections
		if (ctx->fd[0] != STDIN_FILENO)
		{
			dup2(ctx->fd[0], STDIN_FILENO);
			//fprintf(stderr, "exec_command3\n");
			close(ctx->fd[0]);
		}
		if (ctx->fd[1] != STDOUT_FILENO)
		{
			dup2(ctx->fd[1], STDOUT_FILENO);
			//fprintf(stderr, "exec_command4\n");
			close(ctx->fd[1]);
		}
		execvp(node->data.exec_u.args[0], node->data.exec_u.args);
		perror("execvp");
		exit(127);

    }
	if (ctx->fd[0] != STDIN_FILENO)
        close(ctx->fd[0]);
    if (ctx->fd[1] != STDOUT_FILENO)
        close(ctx->fd[1]);
    waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
    {
        return WEXITSTATUS(status);
    }
    return 0;
    //return WEXITSTATUS(status);
}

static int exec_pipe(t_tree_node *node, t_context *ctx)
{
    t_pipecmd *pcmd = &node->data.pipe_u;
    int pipefd[2];
    t_context left_ctx = *ctx;
    t_context right_ctx = *ctx;
    int right_status;
    pid_t left_pid;
    pid_t right_pid;

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

    // Wait for both processes but only use right side's status
    waitpid(left_pid, NULL, 0);  // Ignore left side's status
    waitpid(right_pid, &right_status, 0);
	//return(right_ctx.ret_exit);
	if (WIFEXITED(right_status) && WEXITSTATUS(right_status) != 0)
    {
        return WEXITSTATUS(right_status);
    }
    return 0;
    //return WEXITSTATUS(right_status);
}

void cleanup(t_tree_node *node, int exit_code)
{
	free_tree_node(node);
	clear_history();
	exit(exit_code);
}

int handle_heredoc(t_redircmd *rcmd)
{
	ssize_t bytes_written;

	if (pipe(rcmd->heredoc_pipe) == -1)
	{
		perror("pipe");
		return -1;
	}

    rcmd->heredoc_pid = fork();
	if (rcmd->heredoc_pid == -1)
    {
        perror("fork");
        close(rcmd->heredoc_pipe[0]);
        close(rcmd->heredoc_pipe[1]);
        return -1;
    }
    if (rcmd->heredoc_pid == 0)
    {
        close(rcmd->heredoc_pipe[0]);
        if (rcmd->heredoc_content)
		{
			size_t content_len = ft_strlen(rcmd->heredoc_content);
			bytes_written = write(rcmd->heredoc_pipe[1], rcmd->heredoc_content, content_len);
			if (bytes_written == -1 || bytes_written != (ssize_t)content_len)
			{
				perror("write");
				exit(1);
			}
		}
        close(rcmd->heredoc_pipe[1]);
        exit(0);
    }

    close(rcmd->heredoc_pipe[1]);  // Close write end

        // Redirect stdin to read end of pipe
    if (dup2(rcmd->heredoc_pipe[0], STDIN_FILENO) == -1)
     {
		perror("dup2");
        close(rcmd->heredoc_pipe[0]);
        return -1;
    }
    close(rcmd->heredoc_pipe[0]);

        // Wait for child to finish writing
    int status;
    waitpid(rcmd->heredoc_pid, &status, 0);

	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
    {
        return -1;
    }
    return 0;
}

/* static int exec_redir(t_tree_node *node, t_context *ctx)
{
    t_redircmd *rcmd = &node->data.redir_u;
    int saved_stdin, saved_stdout;
    int result = 0;
    int redir_failed = 0;

    if (save_std_fds(&saved_stdin, &saved_stdout, node) != 0)
        return 1;

    if (setup_pipe_redirection(ctx, saved_stdin, saved_stdout, node) != 0)
    {
        restore_std_fds(saved_stdin, saved_stdout, node);
        return 1;
    }

    if (apply_redirection(rcmd, saved_stdin, saved_stdout, node) != 0)
        redir_failed = 1;

    if (rcmd->cmd == NULL)
    {
        restore_std_fds(saved_stdin, saved_stdout, node);
        return redir_failed;
    }

    // Only execute command if redirection succeeded or we're in a pipeline
    if (!redir_failed || rcmd->cmd->type == N_PIPE)
        result = exec_node(rcmd->cmd, ctx);
    else
        result = 1;

    restore_std_fds(saved_stdin, saved_stdout, node);
    return result;
} */

/* static int exec_redir(t_tree_node *node, t_context *ctx)
{
    t_redircmd *rcmd = &node->data.redir_u;
    int saved_stdin, saved_stdout;
    int redir_failed = 0;

    if (save_std_fds(&saved_stdin, &saved_stdout, node) != 0)
        return 1;

    if (setup_pipe_redirection(ctx, saved_stdin, saved_stdout, node) != 0)
        return 1;

    // Always attempt redirection, but track if it fails
    if (apply_redirection(rcmd, saved_stdin, saved_stdout, node) != 0)
        redir_failed = 1;

    if (rcmd->cmd == NULL)
    {
        restore_std_fds(saved_stdin, saved_stdout, node);
        return redir_failed;
    }

    // If redirection failed but we're in a pipeline, continue execution
    if (redir_failed && rcmd->cmd->type != N_PIPE)
    {
        restore_std_fds(saved_stdin, saved_stdout, node);
        return 1;
    }

    int result = exec_node(rcmd->cmd, ctx);
    restore_std_fds(saved_stdin, saved_stdout, node);
    return result;
} */

/* static int exec_redir(t_tree_node *node, t_context *ctx)
{
    t_redircmd *rcmd = &node->data.redir_u;
    int saved_stdin, saved_stdout;
    int result = 0;

    if (save_std_fds(&saved_stdin, &saved_stdout, node) != 0)
        return 1;

    if (setup_pipe_redirection(ctx, saved_stdin, saved_stdout, node) != 0)
        return 1;

    // Always validate redirections, regardless of command type
    if (apply_redirection(rcmd, saved_stdin, saved_stdout, node) != 0)
    {
        restore_std_fds(saved_stdin, saved_stdout, node);
        return 1;
    }

    if (rcmd->cmd == NULL)
    {
        restore_std_fds(saved_stdin, saved_stdout, node);
        return 1;
    }

    result = exec_node(rcmd->cmd, ctx);
    restore_std_fds(saved_stdin, saved_stdout, node);
    return result;
} */

/* static int exec_redir(t_tree_node *node, t_context *ctx)
{
    t_redircmd *rcmd = &node->data.redir_u;
    int saved_stdin, saved_stdout;
    int had_input_failure = 0;
    int is_input_sensitive = 0;

    // Check if this is a command that needs stdin
    if (rcmd->cmd && rcmd->cmd->type == N_EXEC)
    {
        const char *cmd = rcmd->cmd->data.exec_u.args[0];
        is_input_sensitive = (strcmp(cmd, "cat") == 0 ||
                            strcmp(cmd, "grep") == 0 ||
                            strcmp(cmd, "wc") == 0);
    }

    if (save_std_fds(&saved_stdin, &saved_stdout, node) != 0)
        return 1;

    if (setup_pipe_redirection(ctx, saved_stdin, saved_stdout, node) != 0)
        return 1;

    // Handle input redirection
    if (rcmd->redir_type == REDIR_IN)
    {
        if (apply_redirection(rcmd, saved_stdin, saved_stdout, node) != 0)
        {
            had_input_failure = 1;
            if (is_input_sensitive)
            {
                restore_std_fds(saved_stdin, saved_stdout, node);
                return 1;
            }
        }
    }
    else if (apply_redirection(rcmd, saved_stdin, saved_stdout, node) != 0)
        return 1;

    // If this is a command node, execute it
    if (rcmd->cmd == NULL)
    {
        restore_std_fds(saved_stdin, saved_stdout, node);
        return had_input_failure && is_input_sensitive;
    }

    int result = exec_node(rcmd->cmd, ctx);
    restore_std_fds(saved_stdin, saved_stdout, node);
    return result;
} */

/* static int exec_redir(t_tree_node *node, t_context *ctx)
{
    t_redircmd *rcmd = &node->data.redir_u;
    int saved_stdin, saved_stdout;
    int had_input_failure = 0;

    if (save_std_fds(&saved_stdin, &saved_stdout, node) != 0)
        return 1;

    if (setup_pipe_redirection(ctx, saved_stdin, saved_stdout, node) != 0)
        return 1;

    // Track input redirection failures
    if (rcmd->redir_type == REDIR_IN)
    {
        if (apply_redirection(rcmd, saved_stdin, saved_stdout, node) != 0)
            had_input_failure = 1;
    }
    else if (apply_redirection(rcmd, saved_stdin, saved_stdout, node) != 0)
        return 1;

    // If this is a command node, execute it only if we had no input failures
    if (rcmd->cmd == NULL)
    {
        restore_std_fds(saved_stdin, saved_stdout, node);
        return had_input_failure;
    }

    // If we had an input failure, propagate it
    if (had_input_failure)
    {
        restore_std_fds(saved_stdin, saved_stdout, node);
        return 1;
    }

    int result = exec_node(rcmd->cmd, ctx);
    restore_std_fds(saved_stdin, saved_stdout, node);
    return result;
} */

/* static int exec_redir(t_tree_node *node, t_context *ctx)
{
    t_redircmd *rcmd = &node->data.redir_u;
    int saved_stdin, saved_stdout, result = 0;
    int is_essential_input = 0;

    // Check if this is an essential input redirection
    if (rcmd->cmd && rcmd->cmd->type == N_EXEC &&
        rcmd->redir_type == REDIR_IN &&
        (strcmp(rcmd->cmd->data.exec_u.args[0], "cat") == 0 ||
         strcmp(rcmd->cmd->data.exec_u.args[0], "grep") == 0 ||
         strcmp(rcmd->cmd->data.exec_u.args[0], "wc") == 0))
    {
        is_essential_input = 1;
    }

    if (save_std_fds(&saved_stdin, &saved_stdout, node) != 0)
        return 1;

    if (setup_pipe_redirection(ctx, saved_stdin, saved_stdout, node) != 0)
        return 1;

    // Handle redirections based on whether they're essential
    if (rcmd->redir_type == REDIR_IN)
    {
        result = redirect_input(rcmd, saved_stdin, saved_stdout, node);
        if (result != 0 && is_essential_input)
        {
            restore_std_fds(saved_stdin, saved_stdout, node);
            return 1;
        }
    }
    else if (apply_redirection(rcmd, saved_stdin, saved_stdout, node) != 0)
        return 1;

    if (rcmd->cmd == NULL)
    {
        restore_std_fds(saved_stdin, saved_stdout, node);
        return 0;
    }

    result = exec_node(rcmd->cmd, ctx);
    restore_std_fds(saved_stdin, saved_stdout, node);
    return result;
} */

/* static int exec_redir(t_tree_node *node, t_context *ctx)
{
    t_redircmd *rcmd = &node->data.redir_u;
    int saved_stdin, saved_stdout, result = 0;

    if (save_std_fds(&saved_stdin, &saved_stdout, node) != 0)
        return 1;

    if (setup_pipe_redirection(ctx, saved_stdin, saved_stdout, node) != 0)
        return 1;

    // For input redirections, don't stop on error
    if (rcmd->redir_type == REDIR_IN)
    {
        result = redirect_input(rcmd, saved_stdin, saved_stdout, node);
        if (result != 0)
        {
            restore_std_fds(saved_stdin, saved_stdout, node);
            return 1;  // Return error code 1 for file not found
        }
    }
    else if (apply_redirection(rcmd, saved_stdin, saved_stdout, node) != 0)
        return 1;

    if (rcmd->cmd == NULL)
    {
        restore_std_fds(saved_stdin, saved_stdout, node);
        return 0;  // No command to execute is not an error
    }

    result = exec_node(rcmd->cmd, ctx);
    restore_std_fds(saved_stdin, saved_stdout, node);
    return result;
} */

/* static int exec_redir(t_tree_node *node, t_context *ctx)
{
	t_redircmd *rcmd = &node->data.redir_u;
	int saved_stdin, saved_stdout, result = 0;

	if (save_std_fds(&saved_stdin, &saved_stdout, node) != 0)
		return 1;

	if (setup_pipe_redirection(ctx, saved_stdin, saved_stdout, node) != 0)
		return 1;

	if (apply_redirection(rcmd, saved_stdin, saved_stdout, node) != 0)
		return 1;

	if (rcmd->cmd == NULL)
	{
		fprintf(stderr, "Error: Command node is NULL\n");
		close(saved_stdin);
		close(saved_stdout);
		cleanup(node, 1);
	}

	result = exec_node(rcmd->cmd, ctx);
	restore_std_fds(saved_stdin, saved_stdout, node);
	return result;
} */
