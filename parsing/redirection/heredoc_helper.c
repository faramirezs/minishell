/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 18:18:13 by alramire          #+#    #+#             */
/*   Updated: 2025/02/15 16:03:37 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "../../headers/env_var.h"
#include "../../headers/minishell.h"

void	cleanup_heredoc(t_redircmd *rcmd)
{
	int	status;

	if (rcmd->heredoc_content)
	{
		free(rcmd->heredoc_content);
		rcmd->heredoc_content = NULL;
	}
	if (rcmd->heredoc_pid > 0)
	{
		waitpid(rcmd->heredoc_pid, &status, 0);
	}
	restore_stdin();
}

void	restore_stdin(void)
{
	int	new_stdin;

	if (isatty(STDIN_FILENO) == 0)
	{
		new_stdin = open("/dev/tty", O_RDONLY);
		if (new_stdin != -1)
		{
			dup2(new_stdin, STDIN_FILENO);
			close(new_stdin);
		}
	}
}

char	*get_var_name(char *var_start)
{
	char	*var_end;
	size_t	var_len;
	char	*var_name;

	var_end = var_start + 1;
	while (ft_isalnum(*var_end) || *var_end == '_')
		var_end++;
	var_len = var_end - var_start - 1;
	var_name = ft_substr(var_start + 1, 0, var_len);
	return (var_name);
}

char	*replace_var_with_value(char *expanded_line, char *var_start,
		char *var_value)
{
	char	*new_line;
	char	*var_end;

	var_end = var_start + 1;
	while (ft_isalnum(*var_end) || *var_end == '_')
		var_end++;
	new_line = ft_strjoin_free_s1(ft_strjoin_free_s1(ft_substr(expanded_line, 0,
					var_start - expanded_line), var_value), var_end);
	free(expanded_line);
	return (new_line);
}

char	*expand_env_vars(const char *line, t_context *msh)
{
	char	*expanded_line;
	char	*var_start;
	char	*var_name;
	char	*var_value;

	expanded_line = ft_strdup(line);
	var_start = ft_strchr(expanded_line, '$');
	while (var_start)
	{
		var_name = get_var_name(var_start);
		var_value = ms_get_env(msh->env, var_name);
		if (var_value)
		{
			expanded_line = replace_var_with_value(expanded_line, var_start,
					var_value);
		}
		free(var_name);
		var_start = ft_strchr(var_start + 1, '$');
	}
	return (expanded_line);
}
