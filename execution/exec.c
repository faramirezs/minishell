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
		else
		{
			cleanup(node, EXIT_SUCCESS);
		}
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


int exec(t_tree_node *node)
{
	t_context ctx;
	int children;
	int status;

	ctx.fd[0] = STDIN_FILENO;
	ctx.fd[1] = STDOUT_FILENO;
	ctx.fd_close = -1;

	children = exec_node(node, &ctx);

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

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return -1;
	}
	if (pid == FORKED_CHILD)
	{
		//printf("Child pID: %d\n", getpid());
		//evaluate the context and act on
		dup2(ctx->fd[STDIN_FILENO], STDIN_FILENO);
		dup2(ctx->fd[STDOUT_FILENO], STDOUT_FILENO);
		if(ctx->fd_close >= 0)
			close(ctx->fd_close);
		//printf("+++Exec_command()check+++\n");
		//check_null_array(node->data.exec_u.args);
		execvp(node->data.exec_u.args[0], node->data.exec_u.args);
		perror("execvp"); // If execvp fails
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
	{
		int status;
        waitpid(pid, &status, 0);
		//printf("Parent pID: %d\n", getpid());
		//New line or no new line, that is the question...
        return (1);
	}
	else
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}

}

static int exec_pipe(t_tree_node *node, t_context *ctx)
{
	t_tree_node *lhs;
	t_tree_node *rhs;
	t_context lhs_ctx;
	t_context rhs_ctx;
	int p[2];
	int children;

	children = 0;
	if (pipe(p) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	// Set up left side of pipe
	lhs = node->data.pipe_u.left;
	lhs_ctx = *ctx;
	lhs_ctx.fd[STDOUT_FILENO] = p[1];
	lhs_ctx.fd_close = p[0];
	children += exec_node(lhs, &lhs_ctx);

	// Close write end in parent
	close(p[1]);

	// Set up right side of pipe
	rhs = node->data.pipe_u.right;
	rhs_ctx = *ctx;
	rhs_ctx.fd[STDIN_FILENO] = p[0];
	rhs_ctx.fd_close = -1;
	children += exec_node(rhs, &rhs_ctx);

	// Close read end in parent
	close(p[0]);

	return (children);
}

void cleanup(t_tree_node *node, int exit_code)
{
/* 	if (node && node->type == N_REDIR)  // Only clean heredoc for redirection nodes
	{
		cleanup_heredoc(&node->data.redir_u);
	} */
	free_tree_node(node);
	clear_history();
	exit(exit_code);
}

int handle_heredoc(t_redircmd *rcmd)
{
	if (pipe(rcmd->heredoc_pipe) == -1)
	{
		perror("pipe");
		return -1;
	}

    rcmd->heredoc_pid = fork();
    if (rcmd->heredoc_pid == 0)
    {
        close(rcmd->heredoc_pipe[0]);
        write(rcmd->heredoc_pipe[1], rcmd->heredoc_content,
              ft_strlen(rcmd->heredoc_content));
		//cleanup_heredoc(rcmd);
		//free(rcmd->heredoc_content);
		close(rcmd->heredoc_pipe[1]);
        return(0);
		//return (0);
		//When the heredoc finishes correctly this is the exit flow.
    }
    else if (rcmd->heredoc_pid > 0)
    {
        close(rcmd->heredoc_pipe[1]);
        if (dup2(rcmd->heredoc_pipe[0], STDIN_FILENO) == -1)
        {
            perror("dup2");
            return -1;
        }
        close(rcmd->heredoc_pipe[0]);

        // Wait for child process to complete
        int status;
        waitpid(rcmd->heredoc_pid, &status, 0);
        // Ensure proper terminal output
        write(STDOUT_FILENO, "\n", 1);
		cleanup_heredoc(rcmd);
        //restore_terminal_settings();
        return (0);
    }
    return (-1);
}
