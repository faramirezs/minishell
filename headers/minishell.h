/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 15:17:33 by alramire          #+#    #+#             */
/*   Updated: 2024/12/12 11:05:18 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<unistd.h>
#include	<readline/readline.h>
#include	<readline/history.h>
#include	"../libft/libft.h"
#include	"stdbool.h" // ADDED THIS TO HANDLE TRUE / FALSE SITUATIONS - instead than using 0 or 1 (we can use it, yeah?)
#include	<sys/wait.h>

// CALL VALGRIND SUPPRESSION:
// valgrind --suppressions=readline.supp --leak-check=full --show-leak-kinds=all ./minishell

#define COLOR_RESET "\001\033[0m\002"
#define COLOR_RED "\001\033[31m\002"
#define COLOR_GREEN "\001\033[32m\002"
#define COLOR_YELLOW "\001\033[33m\002"
#define COLOR_BLUE "\001\033[34m\002"
#define COLOR_MAGENTA "\001\033[35m\002"
#define COLOR_CYAN "\001\033[36m\002"
#define COLOR_WHITE "\001\033[37m\002"
#define COLOR_BOLD "\001\033[1m\002"

// Parsing headers
#include	"token.h" // token structs and functions
//#include	"node.h" // node structs and node handling functions
#include	"tree_node.h"
#include	"tree_parser.h"

//Execution headers
#include	"built_in.h" // this already should include all the builtins headers inside
#include	"env_var.h" // this already should include all the env_var headers inside
#include	"pipes.h" // this already should include all the pipes headers inside
#include	"redirections.h" // this already should include all the redirections headers inside
#include	"exec.h"

//Tokenizer headers
#include "char_itr.h"
#include "guards.h"
#include "scanner.h"
#include "cmd.h"
#include "token_list.h"

#endif
