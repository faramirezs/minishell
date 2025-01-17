/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 15:17:33 by alramire          #+#    #+#             */
/*   Updated: 2025/01/17 15:51:24 by alramire         ###   ########.fr       */
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
//# include <termios.h>
// CALL VALGRIND SUPPRESSION:
// valgrind --suppressions=readline.supp --leak-check=full --show-leak-kinds=all ./minishell

# define COLOR_RESET "\001\033[0m\002"
# define COLOR_RED "\001\033[31m\002"
# define COLOR_GREEN "\001\033[32m\002"
# define COLOR_YELLOW "\001\033[33m\002"
# define COLOR_BLUE "\001\033[34m\002"
# define COLOR_MAGENTA "\001\033[35m\002"
# define COLOR_CYAN "\001\033[36m\002"
# define COLOR_WHITE "\001\033[37m\002"
# define COLOR_BOLD "\001\033[1m\002"

// Parsing headers
# include "token.h" // token structs and functions
//#include	"node.h" // node structs and node handling functions
# include "tree_node.h"
# include "tree_parser.h"

// Args collector
# include "args.h"

// Execution headers
# include "built_in.h"
	// this already should include all the builtins headers inside
# include "env_var.h"
	// this already should include all the env_var headers inside
# include "exec.h"
# include "pipes.h"
	// this already should include all the pipes headers inside
# include "redirections.h"
	// this already should include all the redirections headers inside

// Tokenizer headers
# include "char_itr.h"
# include "cmd.h"
# include "guards.h"
# include "scanner.h"

//Signals
# include "signals.h"

#endif
