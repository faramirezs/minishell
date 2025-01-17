#ifndef BUILT_IN_H
# define BUILT_IN_H

# include "minishell.h"

typedef struct bld_in {
    char    *name;               // Name of the builtin (e.g., "echo")
    int     (*func)(struct s_tree_node *node, t_context *msh);  // Function pointer to the builtin's handler
    struct bld_in *next;         // Linked list for multiple builtins
} bld_in;

// Function prototypes
bld_in	*initialize_builtins(void);
bld_in	*find_builtin(bld_in *head, const char *name);
bld_in  *create_builtin_list(void);
int		handle_cd(struct s_tree_node *node, t_context *msh)
int		handle_echo(struct s_tree_node *node, t_context *msh);
int		handle_exit(struct s_tree_node *node, t_context *msh);
int		handle_export(struct s_tree_node *node, t_context *msh);
int		handle_pwd(struct s_tree_node *node, t_context *msh);

int 	handle_env(struct s_tree_node *node, t_context *msh);
void	free_builtin_list(bld_in *head);
int     is_builtin(t_tree_node *node);
void    execute_builtin(t_tree_node *node, t_context *ctx);
void 	add_builtin(bld_in **head, const char *name, int (*func)(struct s_tree_node *node, t_context *msh));

#endif

