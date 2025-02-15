/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 17:10:32 by alramire          #+#    #+#             */
/*   Updated: 2025/02/15 17:10:35 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>
# include <sys/types.h>
# include <errno.h>
# include <signal.h>
# include "token.h"
# include "char_itr.h"
# include "scanner.h"
# include "args.h"
# include "tree_node.h"
# include "env_var.h"
# include "built_in.h"
# include "exec.h"
# include "redirections.h"
# include "cmd.h"
# include "guards.h"
# include "signals.h"

# define COLOR_RESET "\001\033[0m\002"
# define COLOR_RED "\001\033[31m\002"
# define COLOR_GREEN "\001\033[32m\002"
# define COLOR_YELLOW "\001\033[33m\002"
# define COLOR_BLUE "\001\033[34m\002"
# define COLOR_MAGENTA "\001\033[35m\002"
# define COLOR_CYAN "\001\033[36m\002"
# define COLOR_WHITE "\001\033[37m\002"
# define COLOR_BOLD "\001\033[1m\002"
# define WHITESPACES " \t\r\n\a"
# define SYMBOLS "\\;()#&`*~|"
# define NOBRKSYMS "_.?:-+=^"
# define QUOTEETC "\"'"

extern volatile sig_atomic_t g_heredoc_interrupt;

#endif
