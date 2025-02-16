/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 11:32:54 by alramire          #+#    #+#             */
/*   Updated: 2025/02/14 18:15:49 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/minishell.h"

volatile sig_atomic_t	g_heredoc_interrupt = 0;

void	handle_sigint(int sig)
{
	if (sig == SIGINT)
	{
		if (g_heredoc_interrupt)
		{
			g_heredoc_interrupt = 1;
			write(STDOUT_FILENO, "\n", 1);
		}
		else
		{
			write(STDOUT_FILENO, "\n\033[0;32mMinishell> \033[0m", 23);
		}
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
