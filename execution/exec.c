#include "../headers/minishell.h"

#define FORKED_CHILD 0


/* Exit Codes and meaning (we should remember to assign them after parsing and executing)
	0 	- Success: Command executed successfully.
	1 	- General error: Command failed for a generic reason.
	2 	- Incorrect usage: Invalid arguments or syntax in the command (Jess can add this in main.c - if(!node_list) after the node is freed)
	126	- Cannot execute: File exists but is not executable.
	127	- Command not found: Command is missing in the system's PATH.
	130	- Script interrupted (SIGINT): Process terminated via Ctrl+C. */

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
	int fd = open(rcmd->target, O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		close(saved_stdin);
		close(saved_stdout);
		return 1;
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		close(saved_stdin);
		close(saved_stdout);
		cleanup(node, 1);
	}
	close(fd);
	return 0;
}

static int redirect_output(t_redircmd *rcmd, int saved_stdin, int saved_stdout, t_tree_node *node)
{
	int fd = open(rcmd->target, rcmd->flags, rcmd->mode);
	if (fd < 0)
	{
		perror("open");
		close(saved_stdin);
		close(saved_stdout);
		cleanup(node, 1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		close(saved_stdin);
		close(saved_stdout);
		cleanup(node, 1);
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
	if (dup2(saved_stdin, STDIN_FILENO) == -1 || dup2(saved_stdout, STDOUT_FILENO) == -1)
	{
		perror("dup2");
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

	//fprintf(stderr, "DEBUG: children before command = %d\n", children);

	// Wait for all child processes to complete
	while (children > 0) {
		wait(&status);
		children--;
	}
	//fprintf(stderr, "DEBUG: children after builtin = %d\n", children);
	//printf ("the status is %d\n", msh->ret_exit);
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
			//printf("Executing builtin\n");
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
			close(ctx->fd[0]);
		}
		if (ctx->fd[1] != STDOUT_FILENO)
		{
			dup2(ctx->fd[1], STDOUT_FILENO);
			close(ctx->fd[1]);
		}
		execvp(node->data.exec_u.args[0], node->data.exec_u.args);
		perror("execvp");
		return(127);

    }
	if (ctx->fd[0] != STDIN_FILENO)
        close(ctx->fd[0]);
    if (ctx->fd[1] != STDOUT_FILENO)
        close(ctx->fd[1]);
    waitpid(pid, &status, 0);
    return WEXITSTATUS(status);
}

static int exec_pipe(t_tree_node *node, t_context *ctx)
{
	t_pipecmd *pcmd = &node->data.pipe_u;
    int pipefd[2];
    t_context left_ctx = *ctx;
    t_context right_ctx = *ctx;
    int status;
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
        close(pipefd[0]);  // Close unused read end
        status = exec_node(pcmd->left, &left_ctx);
        exit(status);
    }

    // Fork for right side
    right_pid = fork();
    if (right_pid == 0)
    {
        close(pipefd[1]);  // Close unused write end
        status = exec_node(pcmd->right, &right_ctx);
        exit(status);
    }

    close(pipefd[0]);
    close(pipefd[1]);

    waitpid(left_pid, &status, 0);
    waitpid(right_pid, &status, 0);

    return WEXITSTATUS(status);
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

