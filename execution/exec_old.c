#include "../headers/minishell.h"

// static int exec_node(t_tree_node *node, t_context *ctx);
// int handle_heredoc(t_redircmd *rcmd);
// void cleanup(t_tree_node *node, int exit_code);

// static void safe_dup2(int oldfd, int newfd)
// {
//     if (oldfd != newfd)
//     {
//         dup2(oldfd, newfd);
//         close(oldfd);
//     }
// }
// static int exec_command(t_tree_node *node, t_context *ctx)
// {
//     pid_t pid;
//     int status;

//     if (is_builtin(node) && ctx->fd[0] == STDIN_FILENO && ctx->fd[1] == STDOUT_FILENO)
//         return (ctx->ret_exit = execute_builtin(node, ctx));

//     pid = fork();
//     if (pid == -1)
//         return (perror("fork"), -1);

//     if (pid == 0) // Child process
//     {
//         safe_dup2(ctx->fd[0], STDIN_FILENO);
//         safe_dup2(ctx->fd[1], STDOUT_FILENO);
//         if (is_builtin(node))
//             exit(execute_builtin(node, ctx));
//         execvp(node->data.exec_u.args[0], node->data.exec_u.args);
//         perror("execvp");
//         exit(127);
//     }

//     waitpid(pid, &status, 0);
//     ctx->ret_exit = WEXITSTATUS(status);
//     return (ctx->ret_exit);
// }

// static int exec_pipe(t_tree_node *node, t_context *ctx)
// {
//     t_pipecmd *pcmd = &node->data.pipe_u;
//     int pipefd[2];
//     pid_t left_pid, right_pid;
//     int left_status, right_status;

//     if (pipe(pipefd) == -1)
//     {
//         perror("pipe");
//         return -1;
//     }
//     left_pid = fork();
//     if (left_pid == 0)
//     {
//         close(pipefd[0]);  // Left process only writes
//         dup2(pipefd[1], STDOUT_FILENO);
//         close(pipefd[1]);
//         exit(exec_node(pcmd->left, ctx));
//     }
//     close(pipefd[1]);  // Parent closes write end after left process forks

//     right_pid = fork();
//     if (right_pid == 0)
//     {
//         dup2(pipefd[0], STDIN_FILENO);
//         close(pipefd[0]);
//         exit(exec_node(pcmd->right, ctx));
//     }
//     close(pipefd[0]);  // Parent closes read end after right process forks

//     waitpid(left_pid, &left_status, 0);
//     waitpid(right_pid, &right_status, 0);
//     return (WEXITSTATUS(right_status));
// }


// static int redirect_input(t_redircmd *rcmd)
// {
//     int fd;

// 	fd = open(rcmd->target, O_RDONLY);
//     if (fd < 0)
//     {
//         perror(rcmd->target);
//         return 1;
//     }
//     dup2(fd, STDIN_FILENO);
//     close(fd);
//     return 0;
// }

// static int redirect_output(t_redircmd *rcmd)
// {
//     int fd;

// 	fd = open(rcmd->target, rcmd->flags, rcmd->mode);
//     if (fd < 0)
//     {
//         perror(rcmd->target);
//         return 1;
//     }
//     dup2(fd, STDOUT_FILENO);
//     close(fd);
//     return 0;
// }

// static int apply_redirection(t_redircmd *rcmd)
// {
//     if (rcmd->redir_type == REDIR_IN)
//         return (redirect_input(rcmd));
//     if (rcmd->redir_type == REDIR_OUT || rcmd->redir_type == APPEND_OUT)
//         return (redirect_output(rcmd));
//     if (rcmd->redir_type == HEREDOC)
//         return (handle_heredoc(rcmd));
//     return 0;
// }

// static int exec_redir(t_tree_node *node, t_context *ctx)
// {
//     t_redircmd *rcmd;
//     int saved_stdin;
//     int saved_stdout;
//     int result;

// 	rcmd = &node->data.redir_u;
// 	saved_stdin = dup(STDIN_FILENO);
// 	saved_stdout = dup(STDOUT_FILENO);
//     if (saved_stdin == -1 || saved_stdout == -1)
//         return (perror("dup"), 1);
//     if (apply_redirection(rcmd) != 0)
//     {
//         dup2(saved_stdin, STDIN_FILENO);
//         dup2(saved_stdout, STDOUT_FILENO);
//         close(saved_stdin);
//         close(saved_stdout);
//         return 1;
//     }
//     if (rcmd->cmd)
//     	result = exec_node(rcmd->cmd, ctx);
// 	else
//     	result = 0;
//     dup2(saved_stdin, STDIN_FILENO);
//     dup2(saved_stdout, STDOUT_FILENO);
//     close(saved_stdin);
//     close(saved_stdout);
//     return (result);
// }


// int exec(t_tree_node *node, t_context *msh)
// {
//     int status;
//     pid_t pid;

//     if (is_builtin(node) && msh->fd[0] == STDIN_FILENO && msh->fd[1] == STDOUT_FILENO)
//     {
//         return (msh->ret_exit = execute_builtin(node, msh));
//     }

//     pid = fork();
//     if (pid == -1)
//     {
//         perror("fork");
//         return -1;
//     }
//     if (pid == 0) // Child process
//     {
//         exit(exec_node(node, msh));
//     }

//     waitpid(pid, &status, 0);
//     msh->ret_exit = WEXITSTATUS(status);
//     return msh->ret_exit;
// }


// static int exec_node(t_tree_node *node, t_context *ctx)
// {
//     if (node->type == N_EXEC)
//         return exec_command(node, ctx);
//     if (node->type == N_PIPE)
//         return exec_pipe(node, ctx);
//     if (node->type == N_REDIR)
//         return exec_redir(node, ctx);
//     return 0;
// }

// int handle_heredoc(t_redircmd *rcmd)
// {
// 	ssize_t bytes_written;
// 	size_t total_written;
// 	int status;
// 	size_t content_len;

// 	if (pipe(rcmd->heredoc_pipe) == -1)
// 		return (perror("pipe"), -1);

// 	rcmd->heredoc_pid = fork();
// 	if (rcmd->heredoc_pid == -1)
// 		return (perror("fork"), -1);

// 	if (rcmd->heredoc_pid == 0)
// 	{
// 		close(rcmd->heredoc_pipe[0]);
// 		if (!rcmd->heredoc_content)
// 			write(rcmd->heredoc_pipe[1], "\n", 1);  // ✅ Handle empty input properly
// 		else
// 		{
// 			content_len = ft_strlen(rcmd->heredoc_content);
// 			total_written = 0;
// 			while (total_written < content_len)
// 			{
// 				bytes_written = write(rcmd->heredoc_pipe[1], rcmd->heredoc_content + total_written, content_len - total_written);
// 				if (bytes_written == -1)
// 				{
// 					perror("write");
// 					exit(1);
// 				}
// 				total_written += bytes_written;
// 			}
// 		}
// 		close(rcmd->heredoc_pipe[1]);
// 		exit(0);
// 	}

// 	close(rcmd->heredoc_pipe[1]);
// 	waitpid(rcmd->heredoc_pid, &status, 0);
// 	return (WIFEXITED(status) && WEXITSTATUS(status) == 0) ? 0 : -1;
// }

// void cleanup(t_tree_node *node, int exit_code)
// {
// 	free_tree_node(node);
// 	clear_history();
// 	exit(exit_code);
// }
