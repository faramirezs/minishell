/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 18:36:54 by alramire          #+#    #+#             */
/*   Updated: 2025/02/09 15:23:20 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

t_tree_node *create_redir_node(t_scanner *scanner)
{
	t_tree_node *redir_node = OOM_GUARD(malloc(sizeof(t_tree_node)), __FILE__, __LINE__);
	redir_node->type = N_REDIR;
	redir_node->data.redir_u.cmd = NULL;
	redir_node->data.redir_u.redir_type = scanner->next.type;
	if (scanner->next.type == REDIR_IN || scanner->next.type == HEREDOC)
		redir_node->data.redir_u.source_fd = STDIN_FILENO;
	else
		redir_node->data.redir_u.source_fd = STDOUT_FILENO;
	redir_node->data.redir_u.flags = get_redir_flags(scanner->next.type);
	redir_node->data.redir_u.mode = 0644;  // Default file permissions
	return redir_node;
}

void handle_redir_heredoc(t_redircmd *redir_node, t_scanner *scanner)
{
	t_list *heredoc_list = NULL;
	while (redir_node->redir_type == HEREDOC)
	{
		char *heredoc_input = collect_heredoc_input(redir_node->target);
		if (!heredoc_input)
		{
			redir_node->heredoc_content = ft_strdup("");
			redir_node->heredoc_pid = -1;
			//free_args(&cmd_args);
			//cleanup(redir_node, EXIT_FAILURE);
			printf("In handle_redir_heredoc: !heredoc_input\n");
		}
		append_node(&heredoc_list, heredoc_input);
		free(heredoc_input);
		redir_node->heredoc_content = concatenate_lines(heredoc_list);
		if (!scanner_has_next(scanner))
			break;
		scanner->next = scanner_next(scanner);
		if (scanner->next.type != HEREDOC)
			break;

		if (!scanner_has_next(scanner))
		{
			printf("Syntax error: nothing after redirection token\n");
			break;
			//cleanup(redir_node, EXIT_FAILURE);
		}
		scanner->next = scanner_next(scanner);
		t_args *file_args = OOM_GUARD(malloc(sizeof(t_args)), __FILE__, __LINE__);
		file_args->count = OOM_GUARD(malloc(sizeof(int)), __FILE__, __LINE__);
		*(file_args->count) = 1;
		args_collector(&scanner->next, file_args);
		redir_node->target = ft_strdup(file_args->words[0]);
		free_args(&file_args);
		redir_node->target_type = determine_target_type(redir_node->target);
	}
	free_list(heredoc_list);
}


/* void handle_redir_heredoc(t_redircmd *redir_node)
{
	char *heredoc_input = collect_heredoc_input(redir_node->target);
	if (!heredoc_input)
	{
		redir_node->heredoc_content = ft_strdup("");
		redir_node->heredoc_pid = -1;
		//cleanup(redir_node, EXIT_FAILURE);
	}
	redir_node->heredoc_content = heredoc_input;
} */

void parse_redir_target(t_scanner *scanner, t_tree_node *redir_node, t_args *file_args)
{
	if (!scanner_has_next(scanner))
	{
		printf("Syntax error: nothing after redirection token\n");
		cleanup(redir_node, EXIT_FAILURE);
	}
	scanner->next = scanner_next(scanner);
	*(file_args->count) = 1;
	args_collector(&scanner->next, file_args);
	redir_node->data.redir_u.target = ft_strdup(file_args->words[0]);
	free_args(&file_args);
	redir_node->data.redir_u.target_type = determine_target_type(redir_node->data.redir_u.target);
	if (redir_node->data.redir_u.redir_type == HEREDOC)
		handle_redir_heredoc(&redir_node->data.redir_u, scanner);
}

t_tree_node *handle_pipe(t_scanner *scanner, t_tree_node *redir_node, t_args *cmd_args)
{
	t_tree_node *pipe_node = OOM_GUARD(malloc(sizeof(t_tree_node)), __FILE__, __LINE__);
	pipe_node->type = N_PIPE;
	if (cmd_args && cmd_args->words != NULL)
	{
		redir_node->data.redir_u.cmd = parse_exec(cmd_args);
	}
	pipe_node->data.pipe_u.left = redir_node;
	pipe_node->data.pipe_u.right = parse_tree_node(scanner);
	return pipe_node;
}

t_tree_node *parse_redir(t_scanner *scanner, t_args *cmd_args)
{
	t_tree_node *redir_node = create_redir_node(scanner);
	t_args *file_args = OOM_GUARD(malloc(sizeof(t_args)), __FILE__, __LINE__);
	file_args->count = OOM_GUARD(malloc(sizeof(int)), __FILE__, __LINE__);
	parse_redir_target(scanner, redir_node, file_args);

	if (scanner_has_next(scanner))
	{
		scanner->next = scanner_next(scanner);
		if (check_redir(scanner))
		{
			redir_node->data.redir_u.cmd = parse_redir(scanner, cmd_args);
		}
		else if (scanner->next.type == PIPE)
		{
			return handle_pipe(scanner, redir_node, cmd_args);
		}
		else
		{
			(*(cmd_args->count))++;
			args_collector(&scanner->next, cmd_args);
			while (scanner_has_next(scanner))
			{
				if (scanner->next.type == PIPE)
				{
					return handle_pipe(scanner, redir_node, cmd_args);
				}
				else
				{
					scanner->next = scanner_next(scanner);
					(*(cmd_args->count))++;
					args_collector(&scanner->next, cmd_args);
				}
			}
			redir_node->data.redir_u.cmd = parse_exec(cmd_args);
		}
	}
	else if (cmd_args && cmd_args->words != NULL)
	{
		redir_node->data.redir_u.cmd = parse_exec(cmd_args);
	}
	return redir_node;
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
