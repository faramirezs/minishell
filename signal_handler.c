#include "headers/minishell.h"

volatile sig_atomic_t g_heredoc_interrupt = 0;

void handle_sigint(int sig)
{
    if (sig == SIGINT)
    {
        if (g_heredoc_interrupt)
        {
            // Handle SIGINT during heredoc
            g_heredoc_interrupt = 1;
            write(STDOUT_FILENO, "\n", 1);
        }
        else
        {
            // Handle SIGINT during normal command execution
            write(STDOUT_FILENO, "\nMinishell> ", 12);
        }
    }
}

void setup_global_signals(void)
{
    signal(SIGINT, handle_sigint);
    signal(SIGQUIT, SIG_IGN); // Ignore Ctrl+slash
}

void setup_heredoc_signals(void)
{
    g_heredoc_interrupt = 0;
    signal(SIGINT, handle_sigint); // Handle Ctrl+C
    signal(SIGQUIT, SIG_IGN);      // Ignore Ctrl+slash
}

void restore_global_signals(void)
{
    signal(SIGINT, handle_sigint);
    signal(SIGQUIT, SIG_IGN); // Ignore Ctrl+slash
}

int is_heredoc_interrupted(void)
{
    return g_heredoc_interrupt;
}

void reset_heredoc_interrupt(void)
{
    g_heredoc_interrupt = 0;
}
