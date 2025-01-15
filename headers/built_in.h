#ifndef BUILT_IN_H
# define BUILT_IN_H

# include "minishell.h"

typedef struct bld_in {
    char    *name;               // Name of the builtin (e.g., "echo")
    int     (*func)(char **av);  // Function pointer to the builtin's handler
    struct bld_in *next;         // Linked list for multiple builtins
} bld_in;

// Function prototypes
bld_in	*initialize_builtins(void);
bld_in	*find_builtin(bld_in *head, const char *name);
void	add_builtin(bld_in **head, const char *name, int (*func)(char **av));
int		handle_cd(char **av);
int		handle_echo(char **av);
int		handle_exit(char **av);
int		handle_export(char **av);
int		handle_pwd(char **av);
int		handle_unset(char **av);
int		handle_env(char **av);
void	free_builtins(bld_in *head);
int     is_builtin(t_tree_node *node);
void    execute_builtin(t_tree_node *node, t_context *ctx);

#endif

