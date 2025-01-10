
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
