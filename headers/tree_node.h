/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_node.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 16:59:28 by alramire          #+#    #+#             */
/*   Updated: 2025/02/15 16:59:30 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TREE_NODE_H
# define TREE_NODE_H

# include "args.h"
# include "scanner.h"
# include <sys/types.h>

typedef struct s_redircmd	t_redircmd;
typedef struct s_execcmd	t_execcmd;
typedef struct s_pipecmd	t_pipecmd;
typedef union u_node_value	t_node_value;
typedef struct s_tree_node	t_tree_node;

typedef enum e_node_type
{
	N_EXEC,
	N_REDIR,
	N_PIPE
}							t_node_type;

typedef enum e_target_type
{
	TARGET_FILENAME,
	TARGET_PATHNAME,
	TARGET_ENV_PATHNAME,
	TARGET_DELIMITER
}							t_target_type;

struct						s_redircmd
{
	t_token_type			redir_type;
	char					*target;
	t_target_type			target_type;
	int						source_fd;
	int						target_fd;
	int						flags;
	mode_t					mode;
	t_tree_node				*cmd;
	int						error_code;
	char					*heredoc_content;
	int						heredoc_pipe[2];
	pid_t					heredoc_pid;
};

struct						s_execcmd
{
	char					**args;
};

struct						s_pipecmd
{
	t_tree_node				*left;
	t_tree_node				*right;
};

union						u_node_value
{
	t_execcmd				exec_u;
	t_redircmd				redir_u;
	t_pipecmd				pipe_u;
};

struct						s_tree_node
{
	t_node_type				type;
	t_node_value			data;
};

t_tree_node					*parse_tree_node(t_scanner *scanner);
t_tree_node					*parse_exec(t_args *args);
t_tree_node					*parse_pipe(t_scanner *scanner, t_args *args);
void						initialize_args(t_token *token, t_args *args);

void						free_tree_node(t_tree_node *node);

void free_token(t_token *token);
void cleanup_scanner(t_scanner *scanner);

#endif
