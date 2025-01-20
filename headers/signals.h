/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 20:13:08 by alejandrora       #+#    #+#             */
/*   Updated: 2025/01/20 11:49:00 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
#define SIGNALS_H

# include "minishell.h"

void handle_sigint(int sig);
void setup_global_signals(void);
void setup_heredoc_signals(void);
void restore_global_signals(void);
int is_heredoc_interrupted(void);
void reset_heredoc_interrupt(void);

#endif
