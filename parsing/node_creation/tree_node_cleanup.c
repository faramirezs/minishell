/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_node_cleanup.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 17:35:15 by alramire          #+#    #+#             */
/*   Updated: 2025/02/15 12:59:43 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

void	free_redir_node(t_redircmd *redir_u)
{
	if (redir_u->redir_type == HEREDOC)
	{
		cleanup_heredoc(redir_u);
	}
	free(redir_u->target);
	redir_u->target = NULL;
	if (redir_u->cmd)
	{
		free_tree_node(redir_u->cmd);
		redir_u->cmd = NULL;
	}
	//SHOULD THIS FREE THE REDIRECT NODE ITSELF?
}

void	free_pipe_node(t_pipecmd *pipe_u)
{
	free_tree_node(pipe_u->left);
	free_tree_node(pipe_u->right);
}

void	free_exec_node(t_execcmd *exec_u)
{
	free_string_array(&exec_u->args);
}

void	free_wordy_node(t_wordy *word_u)
{
	free_string_array(&word_u->wordy);
}

void	free_tree_node(t_tree_node *node)
{
	if (!node)
		return ;
	if (node->type == N_REDIR)
	{
		free_redir_node(&node->data.redir_u);
	}
	else if (node->type == N_PIPE)
	{
		free_pipe_node(&node->data.pipe_u);
	}
	else if (node->type == N_EXEC)
	{
		free_exec_node(&node->data.exec_u);
	}
	else if (node->type == N_WORD)  
    {
        free_wordy_node(&node->data.word_u);
    }
	free(node);
	node = NULL;
}
