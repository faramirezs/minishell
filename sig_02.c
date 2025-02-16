/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_02.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mestefan <mestefan@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 20:49:40 by mestefan          #+#    #+#             */
/*   Updated: 2025/02/15 20:50:22 by mestefan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/minishell.h"

//volatile sig_atomic_t	g_heredoc_interrupt = 0;

int	is_heredoc_interrupted(void)
{
	return (g_heredoc_interrupt);
}

void	reset_heredoc_interrupt(void)
{
	g_heredoc_interrupt = 0;
}
