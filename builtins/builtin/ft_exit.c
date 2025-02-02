#include "../headers/built_in.h"
#include "../headers/minishell.h"

#include <stdio.h> // For printf, fprintf


static int ms_check_exit_arg(const char *arg, int *exit_code)
{
    int i = 0;
    char *endptr;
    long long num;

    while (arg[i] == ' ') // Skip leading spaces
        i++;
    if (arg[i] == '+' || arg[i] == '-') // Handle optional sign
        i++;
    if (!isdigit(arg[i])) // If first non-sign character isn't a digit
    {
        fprintf(stderr, "minishell: exit: %s: numeric argument required\n", arg);
        *exit_code = 255; // Numeric argument required -> Exit with 255
        return 1;         // Error: Non-numeric argument
    }
    num = strtoll(arg, &endptr, 10); // Convert string to number
    // if (*endptr != '\0' || num < -9223372036854775808LL || num > 9223372036854775807LL) // Invalid number
    // {
    //     fprintf(stderr, "minishell: exit: %s: numeric argument required\n", arg);
    //     *exit_code = 255;
    //     return 1;
    // }
    *exit_code = (int)(num % 256); // Normalize to range [0-255]
    if (*exit_code < 0)
        *exit_code += 256; // Handle negative values
    return 0; // Success
}

int handle_exit(struct s_tree_node *node, t_context *msh)
{
    int exit_code;

    // No arguments: Exit with status 0
    if (!node || !node->data.exec_u.args[1])
    {
        msh->ret_exit = 0;
        printf("exit\n");
        exit(0);
    }

    // Check and validate the first argument
    if (ms_check_exit_arg(node->data.exec_u.args[1], &exit_code))
    {
        msh->ret_exit = 255; // Assign error exit code
        exit(255);           // Exit with 255 for invalid argument
    }

    // If more than one argument is passed, print error but don't exit
    if (node->data.exec_u.args[2] != NULL)
    {
        fprintf(stderr, "minishell: exit: too many arguments\n");
        msh->ret_exit = 1; // Error for too many arguments
        return 1;
    }

    // Valid exit code: Exit with the parsed value
    msh->ret_exit = exit_code;
    printf("exit\n");
    exit(exit_code);
}
