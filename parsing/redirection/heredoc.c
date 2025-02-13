/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 18:18:13 by alramire          #+#    #+#             */
/*   Updated: 2025/02/13 17:16:12 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

/* typedef struct s_list
{
	char *line;
	struct s_list *next;
} t_list; */

static t_list *create_node(const char *line)
{
	t_list *node = malloc(sizeof(t_list));
	if (!node)
		return NULL;
	node->line = strdup(line);
	node->next = NULL;
	return node;
}

void append_node(t_list **head, const char *line)
{
	t_list *new_node = create_node(line);
	if (!new_node)
		return;
	if (!*head)
	{
		*head = new_node;
	}
	else
	{
		t_list *temp = *head;
		while (temp->next)
			temp = temp->next;
		temp->next = new_node;
	}
}

static size_t get_total_length(t_list *head)
{
	size_t total_length = 0;
	while (head)
	{
		total_length += strlen(head->line) + 1; // +1 for newline
		head = head->next;
	}
	return total_length;
}

char *concatenate_lines(t_list *head)
{
	size_t total_length = get_total_length(head);
	char *result = malloc(total_length + 1); // +1 for null terminator
	if (!result)
		return NULL;

	char *ptr = result;
	while (head)
	{
		size_t len = strlen(head->line);
		strcpy(ptr, head->line);
		ptr += len;
		*ptr++ = '\n';
		head = head->next;
	}
	*ptr = '\0';
	return result;
}

void free_list(t_list *head)
{
	while (head)
	{
		t_list *temp = head;
		head = head->next;
		free(temp->line);
		free(temp);
	}
}

char *expand_env_vars(const char *line, t_context *msh)
{
    char *expanded_line = NULL;
    char *var_start = NULL;
    char *var_end = NULL;
    char *var_name = NULL;
    char *var_value = NULL;
    size_t var_len = 0;

    expanded_line = ft_strdup(line);
    var_start = ft_strchr(expanded_line, '$');
    while (var_start)
    {
        var_end = var_start + 1;
        while (ft_isalnum(*var_end) || *var_end == '_')
            var_end++;
        var_len = var_end - var_start - 1;
        var_name = ft_substr(var_start + 1, 0, var_len);
        var_value = expand_env_var_value(var_name, msh);
        if (var_value)
        {
            expanded_line = ft_strjoin_free_s1(ft_strjoin_free_s1(ft_substr(expanded_line, 0, var_start - expanded_line), var_value), var_end);
            free(var_value);
        }
        free(var_name);
        var_start = ft_strchr(var_start + 1, '$');
    }
    return expanded_line;
}

char *collect_heredoc_input(const char *delimiter, t_context *msh)
{
	t_list *head = NULL;
	char *line;
	char *result;

	setup_heredoc_signals();
    reset_heredoc_interrupt();

    while (1)
    {
        line = readline("heredoc> ");
        if (!line || is_heredoc_interrupted())
        {
            if (!line)
                fprintf(stderr, "Minishell: warning: here-document at line 1 delimited by end-of-file (wanted `%s')\n", delimiter);
			free(line);
            break;
		}
		if (ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
		char *expanded_line = expand_env_vars(line, msh);
        append_node(&head, expanded_line);
		//Lets collect here the expanded lines.
		//append_node(&head, line);
		free(line);
	}
	if (is_heredoc_interrupted())
	{
		free_list(head);
        return NULL;
	}
	if (is_heredoc_interrupted())
    {
        free_list(head);
        return NULL;
    }
    result = concatenate_lines(head);
    free_list(head);
    return result;
}

void    cleanup_heredoc(t_redircmd *rcmd)
{
    if (rcmd->heredoc_content)
    {
        free(rcmd->heredoc_content);
        rcmd->heredoc_content = NULL;
    }
    if (rcmd->heredoc_pid > 0)
    {
        int status;
        waitpid(rcmd->heredoc_pid, &status, 0);
    }
	restore_stdin();
}

void    restore_stdin(void)
{
    if (isatty(STDIN_FILENO) == 0)
    {
        int new_stdin = open("/dev/tty", O_RDONLY);
        if (new_stdin != -1)
        {
            dup2(new_stdin, STDIN_FILENO);
            close(new_stdin);
        }
    }
}
