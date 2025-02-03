#include "../headers/minishell.h"

#define FORKED_CHILD 0


/* Exit Codes and meaning (we should remember to assign them after parsing and executing)
	0 	- Success: Command executed successfully.
	1 	- General error: Command failed for a generic reason.
	2 	- Incorrect usage: Invalid arguments or syntax in the command (Jess can add this in main.c - if(!node_list) after the node is freed)
	126	- Cannot execute: File exists but is not executable.
	127	- Command not found: Command is missing in the system's PATH.
	130	- Script interrupted (SIGINT): Process terminated via Ctrl+C. */

//From this video https://www.youtube.com/watch?v=KbhDPYHRqkY&list=PLKUb7MEve0TjHQSKUWChAWyJPCpYMRovO&index=67
// Execute pipe node https://www.youtube.com/watch?v=KbhDPYHRqkY&list=PLKUb7MEve0TjHQSKUWChAWyJPCpYMRovO&index=65

/* typedef struct s_context
{
	int fd[2]; //for stdin and stdout
	int fd_close; // Close an fd? -1 if not
} t_context; */

static int exec_node(t_tree_node *node, t_context *ctx);
static int exec_command(t_tree_node *node, t_context *ctx);
static int exec_pipe(t_tree_node *node, t_context *ctx);
static int exec_redir(t_tree_node *node, t_context *ctx);

// Executing command https://www.youtube.com/watch?v=HzAQCUB9Ifw&list=PLKUb7MEve0TjHQSKUWChAWyJPCpYMRovO&index=63&t=826s
//

static int exec_redir(t_tree_node *node, t_context *ctx)
{
	t_redircmd *rcmd;
	int saved_stdin;
	int saved_stdout;
	int result;
	int fd;


	rcmd = &node->data.redir_u;
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	result = 0;

	if (saved_stdin == -1 || saved_stdout == -1)
	{
		perror("dup");
		cleanup(node, 1);
	}
	if (rcmd->redir_type == REDIR_IN)
	{
		fd = open(rcmd->target, O_RDONLY);
		if (fd < 0)
		{
			perror("open");
			close(saved_stdin);
			close(saved_stdout);
			cleanup(node, 1); // General error
		}
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			perror("dup2");
			close(fd);
			close(saved_stdin);
			close(saved_stdout);
			cleanup(node, 1); // General error
		}
		close(fd);
	}
	else if (rcmd->redir_type == HEREDOC)
    {
        if (handle_heredoc(rcmd) < 0)
        {
            //if it is 0, then we could do the cleanup with node info.
			close(saved_stdin);
            close(saved_stdout);
            cleanup(node, EXIT_FAILURE);
        }
		result = exec_node(rcmd->cmd, ctx);
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
		//else
		//{
		//	cleanup(node, EXIT_SUCCESS);
		//}
    }
	else if (rcmd->redir_type == REDIR_OUT || rcmd->redir_type == APPEND_OUT)
	{
		fd = open(rcmd->target, rcmd->flags, rcmd->mode);
		if (fd < 0)
		{
			perror("open");
			close(saved_stdin);
			close(saved_stdout);
			cleanup(node, 1); // General error
		}
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			close(fd);
			close(saved_stdin);
			close(saved_stdout);
			cleanup(node, 1); // General error
		}
		close(fd);
	}
	if (rcmd->cmd == NULL)
	{
		fprintf(stderr, "Error: Command node is NULL\n");
		close(saved_stdin);
		close(saved_stdout);
		cleanup(node, 1); // General error
	}
	result = exec_node(rcmd->cmd, ctx);
	if (dup2(saved_stdin, STDIN_FILENO) == -1 || dup2(saved_stdout, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(saved_stdin);
		close(saved_stdout);
		cleanup(node, 1);
	}
	close(saved_stdin);
	close(saved_stdout);
	return (result);
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

	children = exec_node(node, msh);

	// Wait for all child processes to complete
	while (children > 0) {
		wait(&status);
		children--;
	}
	return (0);
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
	/* if (is_builtin(node) && ctx->fd[0] == STDIN_FILENO && ctx->fd[1] == STDOUT_FILENO)
        {
            printf("Executing builtin\n");
			ctx->ret_exit = execute_builtin(node, ctx);
			return (ctx->ret_exit);
        }
	printf("Executing $PATH functions\n");
			return execute_builtin(node, ctx);
        } */

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
		// Execute builtin or external command
		/* if (is_builtin(node))
		{
			printf("Executing BUILTIN function\n");
			exit(execute_builtin(node, ctx));
		}
		else */
		//printf("Executing $PATH function\n");
		execvp(node->data.exec_u.args[0], node->data.exec_u.args);
		//perror("execvp");
		exit(127);

    }
	// Parent process
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

    // Set up contexts for left and right commands
    left_ctx.fd[1] = pipefd[1];
    right_ctx.fd[0] = pipefd[0];
	 // Fork for left side
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

    // Parent closes both ends of pipe
    close(pipefd[0]);
    close(pipefd[1]);

    // Wait for both children
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
            bytes_written = write(rcmd->heredoc_pipe[1], rcmd->heredoc_content,
                  ft_strlen(rcmd->heredoc_content));
			if (bytes_written == -1)
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

