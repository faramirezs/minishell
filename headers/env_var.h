#ifndef ENV_VAR_H
# define ENV_VAR_H

# include <stddef.h>
# include <stdbool.h>
# include <limits.h>
# include <stdio.h>

# include "minishell.h"
/*
typedef struct s_context
{
	int fd[2]; //for stdin and stdout
	int fd_close; // Close an fd? -1 if not
	char	**env;		   // Environment variables
	char	**env_export;	// Exported variables
	int	 ret_exit;		// Last command's exit code
	char	*user;		   // Current user's name or shell session info
} t_context;
*/
char		**ms_matrix_add_line(char **matrix, const char *new_line);
char		*ms_get_env(char **env, const char *key);
char		*ms_get_varenv(char **env, char *av);
int 		env_compare(char **env, char **av);
int			ms_set_env(char **env, t_context *msh, const char *value);
char 		**ms_remove_line(char **matrix, int index);
int 		ms_unset_env(t_context *msh, const char *key);
int 		find_env_index(char **env, const char *key);
void 		free_env(char **env);
void 		cleanup_context(t_context *msh);
t_context	*init_context(char **envp);
char		*expand_env_var_value(const char *var_name, t_context *msh);
char		**duplicate_env(char **env);
void		check_shlvl(t_context *msh);

#endif
