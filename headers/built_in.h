/*
Header for builtins in the execution/builtins folder.
Was thinking we could also have a header for each type of builtin:

cd.h
echo.h
exit.h
export.h
pwd.h
unset.h

And group them here like this 👇

mell:
---+++++**********+++++---

pls let's have just one buitin.h header file for all builtins

---+++++**********+++++---
*/

// #ifndef BUILT_IN_H
// # define BUILT_IN_H

// #include	"cd.h"
// #include	"echo.h"
// #include	"exit.h"
// #include	"export.h"
// #include	"pwd.h"
// #include	"unset.h"

// #endif
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

#endif

