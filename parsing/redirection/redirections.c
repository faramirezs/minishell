/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandroramirez <alejandroramirez@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 18:36:54 by alramire          #+#    #+#             */
/*   Updated: 2025/01/13 09:50:17 by alejandrora      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

t_tree_node	*parse_redir(t_scanner *scanner, t_args *cmd_args)
{
	t_tree_node	*redir_node;
	t_args		*file_args;

	redir_node = OOM_GUARD(malloc(sizeof(t_tree_node)), __FILE__, __LINE__);
	redir_node->type = N_REDIR;
	file_args = OOM_GUARD(malloc(sizeof(t_args)), __FILE__, __LINE__);
	file_args->count = OOM_GUARD(malloc(sizeof(int)), __FILE__, __LINE__);
	// Store redirection type
	redir_node->data.redir_u.redir_type = scanner->next.type;
    if(scanner->next.type == REDIR_IN)
		redir_node->data.redir_u.source_fd = STDIN_FILENO;
	else
		redir_node->data.redir_u.source_fd = STDOUT_FILENO;
    redir_node->data.redir_u.flags = get_redir_flags(scanner->next.type);
    redir_node->data.redir_u.mode = 0644;  // Default file permissions

	// Move to the file/delimiter token
	if (!scanner_has_next(scanner))
	{
		printf("Syntax error: nothing after redirection token\n");
		return (NULL);
	}
	scanner->next = scanner_next(scanner);
	*(file_args->count) = 1;
	args_collector(&scanner->next, file_args);
	// Set target and its properties
	redir_node->data.redir_u.target = file_args->words[0];
	redir_node->data.redir_u.target_type = determine_target_type(file_args->words[0]);
	// Handle heredoc case
	if (redir_node->data.redir_u.redir_type == HEREDOC)
	{
		printf("collect_heredoc_input until the delimiter is encountered\n");
		// Collect heredoc input until the delimiter is encountered
		// char *heredoc_input = collect_heredoc_input(file_args->words[0]);
		// redir_node->data.redir_u.file_input = heredoc_input;
	}
	else
	{
		printf("Set execution paths for other redirection types\n");
		// Set execution paths for other redirection types
		// redir_node->data.redir_u.exec_file = get_exec_file(file_args->words[0]);
	}
	printf("Set file handling flags based on redirection type\n");
	redir_node->data.redir_u.flags = get_redir_flags(redir_node->data.redir_u.redir_type);
	//redir_node->data.redir_u.close_fd = 1; // Default to true
	// Store command args if provided
	if (cmd_args)
		redir_node->data.redir_u.cmd = parse_exec(cmd_args);
	// Continue parsing if there are more tokens
	if (scanner_has_next(scanner))
	{
		scanner->next = scanner_next(scanner);
		if (check_redir(scanner))
		{
			return (parse_redir(scanner, cmd_args));
		}
	}
	free(file_args->count);
	free(file_args);
	return (redir_node);
}

int	determine_target_type(const char *target)
{
	if (ft_strchr(target, '/'))
		return (TARGET_PATHNAME);
	if (target[0] == '$')
		return (TARGET_ENV_PATHNAME);
	return (TARGET_FILENAME);
}

int	get_redir_flags(int redir_type)
{
	if (redir_type == REDIR_IN)
		return (O_RDONLY);
	else if (redir_type == REDIR_OUT)
		return (O_WRONLY | O_CREAT | O_TRUNC);
	else if (redir_type == APPEND_OUT)
		return (O_WRONLY | O_CREAT | O_APPEND);
	else if (redir_type == REDIR_IN)
		return (O_RDONLY);
	else if (redir_type == REDIR_IN)
		return (O_RDONLY);
	else
		return (0);
}

int	check_redir(t_scanner *scanner)
{
	return ((scanner->next.type == REDIR_IN || scanner->next.type == REDIR_OUT
			|| scanner->next.type == APPEND_OUT
			|| scanner->next.type == HEREDOC));
}

int handle_input_redirection(t_redircmd *rcmd, t_context *ctx) {
    int fd;

	fd = -1;
    // Handle different target types
    if (rcmd->target_type == TARGET_FILENAME) {
        fd = open(rcmd->target, rcmd->flags, rcmd->mode);
    } else if (rcmd->target_type == TARGET_ENV_PATHNAME) {
		printf("TARGET_ENV_PATHNAME\n");
		// Handle environment variable expansion
        //char *expanded_path = expand_env_var(rcmd->target);
        //fd = open(expanded_path, rcmd->flags, rcmd->mode);
        //free(expanded_path);
    }

    if (fd < 0) {
        rcmd->error_code = errno;
        return -1;
    }

    // Redirect input
    if (dup2(fd, rcmd->source_fd) < 0) {
        rcmd->error_code = errno;
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}
