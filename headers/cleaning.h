# include "minishell.h"

void		free_builtin_list(t_bld_in **head);
void		free_string_array(char ***array);
void		free_args(t_args **args);
void		free_env(char **env);
void		cleanup_context(t_context *msh);
void		cleanup(t_tree_node *node, int exit_code);
void		free_list(t_list *head);
void		cleanup_heredoc(t_redircmd *rcmd);
void		free_tree_node(t_tree_node *node);

