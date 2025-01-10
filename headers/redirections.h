/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 17:40:19 by alramire          #+#    #+#             */
/*   Updated: 2025/01/10 18:26:23 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTIONS_H
# define REDIRECTIONS_H

# include "tree_node.h"

/*
	TARGET_FILENAME, 		// 0 - if token_type of the target is word or cmd
	TARGET_PATHNAME, 		// 1 - if token is REL, ABS path (which can end or not with a file)
	TARGET_ENV_PATHNAME,	// 2 - if token is an env_variable (once we have the nodes we check if the env_var is a valid path, like $PATH/file for example)
	TARGET_DELIMITER		// 3 - if redir_type is HEREDOC
 */
/*
typedef enum e_target_type
{
	TARGET_FILENAME,
	TARGET_PATHNAME,
	TARGET_ENV_PATHNAME,
} t_target_type;
 */

/*
	struct s_redir *prev;
	int     redir_type;      // REDIR_IN, REDIR_OUT, APPEND_OUT, HEREDOC
	int     redir_i;         // Order of redirection
	char    *target;         // Filename or delimiter
	int     target_type;     // TARGET_FILENAME, TARGET_PATHNAME, TARGET_ENV_PATHNAME, TARGET_DELIMITER
	int     target_token_type;
	char	**args;
	// Execution data
	//char    *exec_path;      // Path for execution
	char    *exec_file;      // Actual file to execute on
	char    *file_input;     // File content as single string
	char    **split_input;   // File content as lines

	// File handling
	int     flags;          // O_RDONLY, O_WRONLY, O_APPEND, etc.
	int     fd;            // File descriptor
	int    close_fd;      // Resource management flag
	int     error_code;    // For storing specific error states

	struct s_redir *next;
 */
/*
struct						s_redircmd
{
	struct s_redir			*prev;
	int						redir_type;
	int						redir_i;
	char					*target;
	int						target_type;
	int						target_token_type;
	char					**args;
	char					*exec_file;
	char					*file_input;
	char					**split_input;
	int						flags;
	int						fd;
	int						close_fd;
	int						error_code;

	struct s_redir			*next;
};
 */
t_tree_node					*parse_redir(t_scanner *scanner, t_args *cmd_args);
int							check_redir(t_scanner *scanner);
int							determine_target_type(const char *target);
int							get_redir_flags(int redir_type);
#endif
