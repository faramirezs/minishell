/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 19:21:37 by alramire          #+#    #+#             */
/*   Updated: 2025/03/08 19:42:17 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

#define FORKED_CHILD 0

static void	local_cleanup(struct s_tree_node *node, t_context *ctx)
{
	(void)node;
	free_tree_node(&ctx->origin_node);
	free_builtin_list(&ctx->builtins);
	cleanup_context(ctx->origin_ctx);
	exit (127);
}

static void	setup_child_redirections(t_context *ctx)
{
	if (ctx->fd[0] != STDIN_FILENO)
	{
		dup2(ctx->fd[0], STDIN_FILENO);
		close(ctx->fd[0]);
	}
	if (ctx->fd[1] != STDOUT_FILENO)
	{
		dup2(ctx->fd[1], STDOUT_FILENO);
		close(ctx->fd[1]);
	}
}

static void	exec_minishell(t_tree_node *node, t_context *ctx)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (path)
	{
		path = ft_strjoin_free_s1(path, "/minishell");
		execve(path, node->data.exec_u.args, ctx->env);
		free_tree_node(&node);
		free_builtin_list(&ctx->builtins);
		cleanup_context(ctx);
		free(path);
	}
	path = ft_strjoin_free_s1(path, "/minishell");
	execve(path, node->data.exec_u.args, ctx->env);
	perror("execve");
	free_tree_node(&node);
	free_builtin_list(&ctx->builtins);
	cleanup_context(ctx);
	free(path);
	exit(127);
}

char	*find_in_path(const char *cmd, char *path)
{
	char	**paths;
	char	*full_path;
	int		i;

	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin_free_s1(full_path, cmd);
		if (access(full_path, X_OK) == 0)
		{
			ft_free_tab(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	ft_free_tab(paths);
	return (NULL);
}

void	exec_child_process(t_tree_node *node, t_context *ctx)
{
	char	*cmd_path;

	setup_child_redirections(ctx);
	if (ft_strcmp(node->data.exec_u.args[0], "./minishell") == 0)
		exec_minishell(node, ctx);
	else if (node->data.exec_u.args[0][0] == '/' || 
		node->data.exec_u.args[0][0] == '.')
		execve(node->data.exec_u.args[0], node->data.exec_u.args, ctx->env);
	else
	{
		if (!ms_get_env(ctx->env, "PATH"))
		{
			fprintf(stderr, "%s: No such file or directory\n", 
				node->data.exec_u.args[0]);
			local_cleanup(node, ctx);
		}
		cmd_path = find_in_path(node->data.exec_u.args[0], 
				ms_get_env(ctx->env, "PATH"));
		if (cmd_path)
			execve(cmd_path, node->data.exec_u.args, ctx->env);
		free(cmd_path);
	}
	perror("execvp");
	local_cleanup(node, ctx);
}
