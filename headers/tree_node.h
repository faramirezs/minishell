/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_node.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 17:26:14 by alramire          #+#    #+#             */
/*   Updated: 2025/01/10 19:14:30 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TREE_NODE_H
# define TREE_NODE_H

# include "../headers/args.h"
# include "../headers/scanner.h"

typedef struct s_redircmd	t_redircmd;
typedef struct s_execcmd	t_execcmd;
typedef struct s_pipecmd	t_pipecmd;
typedef union u_node_value	t_node_value;
typedef struct s_tree_node	t_tree_node;

// node selector, like pick one and put it on the node struct (saves memory).
typedef enum e_node_type
{
	// N_ERR = -1, just an idea for now
	N_EXEC,
	N_REDIR,
	N_PIPE
	// N_LIST, //for `;` `&&` and `||`
	// N_BACK
}							t_node_type;

typedef enum e_target_type
{
	TARGET_FILENAME,
	TARGET_PATHNAME,
	TARGET_ENV_PATHNAME,
	TARGET_DELIMITER
}							t_target_type;

struct						s_redircmd {
    t_token_type redir_type;          // Use enum for redirection types (e.g., input, output, append).
    char *target;            // Target file or stream.
    int target_fd;           // Target file descriptor.
    int flags;               // File open flags.
    int error_code;          // Error code during processing.
};

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

/* typedef struct s_listcmd
{
	t_node_type type;
	t_cmd *left;
	t_cmd *right;
}							t_listcmd;

typedef struct s_backcmd
{
	t_node_type				type;
	t_cmd					*cmd;
} t_backcmd; */

void						node_drop(t_tree_node *self);

t_tree_node					*parse_tree_node(t_scanner *scanner);
t_tree_node					*parse_exec(t_args *args);
t_tree_node					*parse_redir(t_scanner *scanner, t_args *args);
t_tree_node					*parse_pipe(t_scanner *scanner, t_args *args);
void						indent_node(size_t spaces);
void						visit_node(const t_tree_node *node, size_t spaces);

#endif
