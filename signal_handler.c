/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 11:32:54 by alramire          #+#    #+#             */
/*   Updated: 2025/03/08 19:48:11 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/minishell.h"

volatile sig_atomic_t	g_heredoc_interrupt = 0;

void	handle_sigint(int sig)
{
	if (sig == SIGINT)
	{
		g_heredoc_interrupt = 1;
		rl_done = 1;
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line ("", 0);
		rl_on_new_line ();
		fprintf(stderr, "Press any key to continue.\n");
	}
}

void	setup_global_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_heredoc_signals(void)
{
	g_heredoc_interrupt = 0;
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	restore_global_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}
