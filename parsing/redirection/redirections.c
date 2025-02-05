/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 18:36:54 by alramire          #+#    #+#             */
/*   Updated: 2025/02/05 18:01:33 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

t_tree_node	*parse_redir(t_scanner *scanner, t_args *cmd_args)
{
	t_tree_node	*redir_node;
	t_args		*file_args;

	redir_node = OOM_GUARD(malloc(sizeof(t_tree_node)), __FILE__, __LINE__);
	redir_node->type = N_REDIR;
	redir_node->data.redir_u.cmd = NULL;
	file_args = OOM_GUARD(malloc(sizeof(t_args)), __FILE__, __LINE__);
	file_args->count = OOM_GUARD(malloc(sizeof(int)), __FILE__, __LINE__);
	redir_node->data.redir_u.redir_type = scanner->next.type;
	if(scanner->next.type == REDIR_IN || scanner->next.type == HEREDOC)
		redir_node->data.redir_u.source_fd = STDIN_FILENO;
	else
		redir_node->data.redir_u.source_fd = STDOUT_FILENO;
    redir_node->data.redir_u.flags = get_redir_flags(scanner->next.type);
    redir_node->data.redir_u.mode = 0644;  // Default file permissions
	if (!scanner_has_next(scanner))
	{
		printf("Syntax error: nothing after redirection token\n");
		cleanup(redir_node, EXIT_FAILURE);
	}
	scanner->next = scanner_next(scanner);
	*(file_args->count) = 1;
	args_collector(&scanner->next, file_args);
	// Set target and its properties
	redir_node->data.redir_u.target = ft_strdup(file_args->words[0]);
	free_args(&file_args);
	redir_node->data.redir_u.target_type = determine_target_type(redir_node->data.redir_u.target);
	// Handle heredoc case
	if (redir_node->data.redir_u.redir_type == HEREDOC)
	{
		char *heredoc_input = collect_heredoc_input(redir_node->data.redir_u.target);
		if (!heredoc_input)
		{
			redir_node->data.redir_u.heredoc_content = ft_strdup("");
			redir_node->data.redir_u.heredoc_pid = -1;
			free_args(&cmd_args);
			cleanup(redir_node, EXIT_FAILURE);
		}
		redir_node->data.redir_u.heredoc_content = heredoc_input;
	}
    // Continue parsing if there are more tokens
	if (cmd_args && cmd_args->words != NULL)
		redir_node->data.redir_u.cmd = parse_exec(cmd_args);

	if (scanner_has_next(scanner))
	{
		scanner->next = scanner_next(scanner);
		if (check_redir(scanner))
		{
			redir_node->data.redir_u.cmd = parse_redir(scanner, cmd_args);
		}
		else
		{
			redir_node->data.redir_u.cmd = parse_exec(cmd_args);
		}
	}
	if (cmd_args && cmd_args->words != NULL)
		redir_node->data.redir_u.cmd = parse_exec(cmd_args);
	//I've done the free in line 45
	//free(file_args->count);
	//free(file_args);
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

/* int handle_input_redirection(t_redircmd *rcmd) {
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
} */
