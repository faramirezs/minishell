#include <stdlib.h>

#include "../../headers/redirections.h"
#include "../../headers/tree_node.h"
#include "../../headers/guards.h"
#include "../../headers/scanner.h"
#include "../../headers/token.h"
#include "../../headers/minishell.h"
#include "../../headers/args.h"

t_tree_node *parse_redir(t_scanner *scanner, t_args *cmd_args)
{
    t_tree_node  *redir_node;
    t_args      *file_args;

    redir_node = OOM_GUARD(malloc(sizeof(t_tree_node)), __FILE__, __LINE__);
    redir_node->type = N_REDIR;
    file_args = OOM_GUARD(malloc(sizeof(t_args)), __FILE__, __LINE__);
    file_args->count = OOM_GUARD(malloc(sizeof(int)), __FILE__, __LINE__);

    // Store redirection type
    redir_node->data.redir_u.redir_type = scanner->next.type;

    // Move to the file/delimiter token
    if (!scanner_has_next(scanner))
    {
        printf("Syntax error: nothing after redirection token\n");
        return NULL;
    }

    scanner->next = scanner_next(scanner);
    *(file_args->count) = 1;
    args_collector(&scanner->next, file_args);

    // Set target and its properties
    redir_node->data.redir_u.target = file_args->words[0];
    redir_node->data.redir_u.target_type = determine_target_type(file_args->words[0]);
    redir_node->data.redir_u.target_token_type = scanner->next.type;

    // Handle heredoc case
    if (redir_node->data.redir_u.redir_type == HEREDOC)
    {
        // Collect heredoc input until the delimiter is encountered
        char *heredoc_input = collect_heredoc_input(file_args->words[0]);
        redir_node->data.redir_u.file_input = heredoc_input;
    }
    else
    {
        // Set execution paths for other redirection types
        redir_node->data.redir_u.exec_file = get_exec_file(file_args->words[0]);
    }

    // Set file handling flags based on redirection type
    redir_node->data.redir_u.flags = get_redir_flags(redir_node->data.redir_u.redir_type);
    redir_node->data.redir_u.close_fd = 1;  // Default to true

    // Store command args if provided
    if (cmd_args)
        redir_node->data.redir_u.args = copy_string_array(cmd_args);

    // Continue parsing if there are more tokens
    if (scanner_has_next(scanner))
    {
        scanner->next = scanner_next(scanner);
        if (scanner->next.type == REDIR_IN || scanner->next.type == REDIR_OUT ||
            scanner->next.type == APPEND_OUT || scanner->next.type == HEREDOC)
        {
            return parse_redir(scanner, cmd_args);
        }
    }

    free(file_args->count);
    free(file_args);
    return redir_node;
}
static int determine_target_type(const char *target)
{
    if (strchr(target, '/'))
        return TARGET_PATHNAME;
    if (target[0] == '$')
        return TARGET_ENV_PATHNAME;
    return TARGET_FILENAME;
}

static int get_redir_flags(int redir_type)
{
    if(redir_type == REDIR_IN)
		return O_RDONLY;
	else if (redir_type == REDIR_OUT)
		return O_WRONLY | O_CREAT | O_TRUNC;
	else if(redir_type == APPEND_OUT)
		return O_WRONLY | O_CREAT | O_APPEND;
	else if(redir_type == REDIR_IN)
		return O_RDONLY;
	else if(redir_type == REDIR_IN)
		return O_RDONLY;
	else
        return 0;
}

