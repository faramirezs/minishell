/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signals.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 18:44:01 by alramire          #+#    #+#             */
/*   Updated: 2025/01/24 18:44:33 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

//volatile sig_atomic_t g_heredoc_interrupt = 0;

//void handle_sigint(int sig); // Forward declaration

/* void    handle_heredoc_sigint(int sig)
{
    (void)sig;
    g_heredoc_interrupt = 1;
    write(STDOUT_FILENO, "\n", 1);
} */

/* void    setup_heredoc_signals(void)
{
    g_heredoc_interrupt = 0;
    signal(SIGINT, handle_heredoc_sigint);   // Handle Ctrl+C
    signal(SIGQUIT, SIG_IGN);                // Ignore Ctrl+slash
} */

/* void restore_global_signals(void)
{
    signal(SIGINT, handle_sigint);
    signal(SIGQUIT, SIG_IGN); // Ignore Ctrl+slash
} */

/* void    restore_stdin(void)
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
} */

/* int     is_heredoc_interrupted(void)
{
	return g_heredoc_interrupt;
}

void	reset_heredoc_interrupt(void)
{
    g_heredoc_interrupt = 0;
} */
