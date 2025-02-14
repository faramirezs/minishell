/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   main.c											 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: alejandroramirez <alejandroramirez@stud	+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/10/30 16:26:03 by jslusark		  #+#	#+#			 */
/*   Updated: 2025/01/13 22:13:54 by alejandrora	  ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "headers/minishell.h"
#include "headers/tree_node.h"

// void	shell_loop(t_context *msh)
// {
// 	char		*line;
// 	t_char_itr	itr;
// 	t_scanner	scanner;
// 	t_tree_node	*tree_node;

// 	while (1)
// 	{
// 		line = readline("minishell> ");
// 		if (!line || *line == '\0' || ft_strspn(line, " \t\n") == strlen(line))
// 		{
// 			free(line);
// 			continue ;
// 		}
// 		add_history(line);
// 		itr = char_itr_value(line, ft_strlen(line));
// 		scanner = scanner_value(itr);
// 		scanner.msh = msh;
// 		tree_node = parse_tree_node(&scanner);
// 		msh->ret_exit = exec(tree_node, msh);
// 		free_tree_node(tree_node);
// 		free(line);
// 	}
// }

// void shell_loop(t_context *msh)
// {
//     char *line;
//     t_char_itr itr;
//     t_scanner scanner;
//     t_tree_node *tree_node;

//     int is_interactive = isatty(STDIN_FILENO);  // Check if input is from terminal

//     while (1)
//     {
//         if (is_interactive)
//             line = readline("minishell> ");
//         else
//             line = get_next_line(STDIN_FILENO);  // Use this for non-interactive mode

//         if (!line)
//         {
//             if (is_interactive)
//                 printf("exit\n");
//             break; // Ctrl-D or EOF exit
//         }

//         if (is_interactive)
//             add_history(line);

//         itr = char_itr_value(line, ft_strlen(line));
//         scanner = scanner_value(itr);
//         scanner.msh = msh;
//         tree_node = parse_tree_node(&scanner);

//         msh->ret_exit = exec(tree_node, msh);

//         free_tree_node(tree_node);
//         free(line);

//         if (!is_interactive) // 🚀 If in non-interactive mode, exit after execution
//             break;
//     }
// }

void shell_loop(t_context *msh)
{
	char *line;
	t_char_itr		itr;
	t_scanner		scanner;
	t_tree_node		*tree_node;
	int stdinout[2];
	stdinout[1] = dup(1);
	stdinout[0] = dup(0);

	setup_global_signals();

	while (1)
	{
		dup2(stdinout[1],1);
		dup2(stdinout[0], 0);
		line = readline(COLOR_GREEN "Minishell> " COLOR_RESET);
		if (line == NULL)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break;
		}
		if (ft_strlen(line) > 0)
		{
			add_history(line);
			itr = char_itr_value(line, ft_strlen(line));
			scanner = scanner_value(itr);
			scanner.msh = msh;
			tree_node = parse_tree_node(&scanner);
			msh->ret_exit = exec(tree_node, msh);

			free_tree_node(tree_node);
			free(line);
		}
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_context	*msh;
	bld_in		*builtins;
	int			ret_exit;

	(void)argc;
	(void)argv;
	msh = init_context(envp);
	builtins = create_builtin_list();
	shell_loop(msh);
	ret_exit = msh->ret_exit;
	cleanup_context(msh);
	free_builtin_list(builtins);
	return (ret_exit);
}
