/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   heredoc_signals.c								  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: alramire <alramire@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2025/01/16 18:44:01 by alramire		  #+#	#+#			 */
/*   Updated: 2025/01/17 13:42:58 by alramire		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "../../headers/minishell.h"

static volatile sig_atomic_t g_heredoc_interrupt = 0;

void	handle_heredoc_sigint(int sig)
{
	(void)sig;
	g_heredoc_interrupt = 1;
	write(STDOUT_FILENO, "\n", 1);
	// Close stdin to force readline to return
	//I need to understand this better
	close(STDIN_FILENO);
}

void	setup_heredoc_signals(void)
{
	signal(SIGINT, handle_heredoc_sigint);   // Handle Ctrl+C
	signal(SIGQUIT, SIG_IGN);				// Ignore Ctrl+ slash./
}

void	cleanup_heredoc(t_redircmd *rcmd)
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
	/* if (isatty(STDIN_FILENO) == 0)
	{
		freopen("/dev/tty", "r", stdin);
	} */
}

int	 is_heredoc_interrupted(void)
{
	return g_heredoc_interrupt;
}

void	reset_heredoc_interrupt(void)
{
	g_heredoc_interrupt = 0;
}
