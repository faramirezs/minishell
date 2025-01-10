
/*
The s_redircmd struct represents a single redirection command in the AST.

Fields:
    - redir_type: Specifies the type of redirection (e.g., input, output, append).
      This uses the t_token_type enum to define the redirection type.
    - target: The target of the redirection. This can be a file name, path, or stream name.
    - target_fd: The file descriptor associated with the target. This specifies
      the descriptor to which the redirection applies (e.g., stdin, stdout).
    - flags: Flags to be used with the open function. Different types of redirections
      (input, output, append) require specific flags for file access.
    - error_code: Used to store error codes that occur during redirection processing.
      This helps in diagnosing issues such as file not found, permission denied, etc.


This structure is designed to integrate into the AST, with each command node potentially
containing a linked list of s_redircmd instances for managing its associated redirections.
*/

struct s_redircmd {
    t_token_type redir_type;          // Use enum for redirection types (e.g., input, output, append).
    char *target;            // Target file or stream.
    int target_fd;           // Target file descriptor.
    int flags;               // File open flags.
    int error_code;          // Error code during processing.
};


/*
Comment
Based on the code provided, I'll help analyze your redirection implementation and suggest improvements. Let's look at your current redirection command structure and implementation approach.

Your current `s_redircmd` structure (from tree_node.h):

```44:50:headers/tree_node.h
struct						s_redircmd {
    t_token_type redir_type;          // Use enum for redirection types (e.g., input, output, append).
    char *target;            // Target file or stream.
    int target_fd;           // Target file descriptor.
    int flags;               // File open flags.
    int error_code;          // Error code during processing.
};
```


This is a good basic structure for handling redirections, but I can suggest some improvements to make it more robust and efficient:

1. **Enhanced Redirection Structure**
Here's a suggested improved version:

```c
struct s_redircmd {
    t_token_type redir_type;     // Current redirection type (REDIR_IN, REDIR_OUT, etc.)
    char *target;                // Target file/delimiter
    t_target_type target_type;   // Type of target (filename, pathname, env var, delimiter)
    int source_fd;               // Source file descriptor (usually 0 for input, 1 for output)
    int target_fd;               // Target file descriptor
    int flags;                   // Open flags for the file
    mode_t mode;                 // File permissions when creating new files
    t_tree_node *cmd;           // Command to be redirected
    int error_code;             // Error tracking
};
```

2. **Key Improvements**:

- Added `source_fd` to explicitly track the source file descriptor
- Added `mode_t mode` for file permissions when creating new files
- Added `cmd` pointer to maintain reference to the command being redirected
- Kept `target_type` enum to differentiate between different target types
- Maintained error tracking for robust error handling

3. **Implementation Strategy**:

```c
static int handle_input_redirection(t_redircmd *rcmd) {
    int fd;

    // Handle different target types
    if (rcmd->target_type == TARGET_FILENAME) {
        fd = open(rcmd->target, rcmd->flags, rcmd->mode);
    } else if (rcmd->target_type == TARGET_ENV_PATHNAME) {
        // Handle environment variable expansion
        char *expanded_path = expand_env_var(rcmd->target);
        fd = open(expanded_path, rcmd->flags, rcmd->mode);
        free(expanded_path);
    }

    if (fd < 0) {
        rcmd->error_code = errno;
        return -1;
    }

    // Redirect input
    if (dup2(fd, rcmd->source_fd) < 0) {
        rcmd->error_code = errno;
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}
```

4. **Integration with AST**:

Your current AST approach using the union for different node types is good. For redirection nodes, you should:

1. Parse the redirection operator and target
2. Create a redirection node
3. Set up the appropriate flags and modes
4. Link it with the command node it's redirecting

Example parsing flow:
```c
t_tree_node *parse_redir(t_scanner *scanner, t_args *args) {
    t_tree_node *node = malloc(sizeof(t_tree_node));
    node->type = N_REDIR;

    // Set up redirection command
    node->data.redir_u.redir_type = scanner->next.type;
    node->data.redir_u.source_fd = (scanner->next.type == REDIR_IN) ? STDIN_FILENO : STDOUT_FILENO;
    node->data.redir_u.flags = get_redir_flags(scanner->next.type);
    node->data.redir_u.mode = 0644;  // Default file permissions

    // Get target
    scanner_next(scanner);  // Move to target token
    node->data.redir_u.target = strdup(scanner->next.lexeme.start);
    node->data.redir_u.target_type = determine_target_type(node->data.redir_u.target);

    return node;
}
```

This implementation provides:
- Clear separation of concerns
- Robust error handling
- Support for different target types
- Proper file descriptor management
- Integration with your existing AST structure

The key is to maintain the hierarchical structure of your AST while providing all necessary information for the execution phase to properly handle the redirections.

Your current implementation is on the right track, but these enhancements will make it more robust and maintainable while handling all the edge cases that can occur with shell redirections.

*/
