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

static void	process_command_line(char *line, t_context *msh)
{
	t_char_itr	itr;
	t_scanner	scanner;
	t_tree_node	*tree_node;

	add_history(line);
	itr = char_itr_value(line, ft_strlen(line));
	scanner = scanner_value(itr);
	scanner.msh = msh;
	tree_node = parse_tree_node(&scanner);
	msh->origin_ctx = msh;
	msh->origin_node = tree_node;
	msh->ret_exit = exec(tree_node, msh);
	free_tree_node(&tree_node);
	free(line);
}

void	shell_loop(t_context *msh)
{
	char		*line;
	int			stdinout[2];

	stdinout[1] = dup(1);
	stdinout[0] = dup(0);
	setup_global_signals();
	while (1)
	{
		dup2 (stdinout[1], 1);
		dup2 (stdinout[0], 0);
		rl_on_new_line();
		rl_replace_line("", 0);
		line = readline(COLOR_GREEN "Minishell> " COLOR_RESET);
		if (line == NULL)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		if (ft_strlen(line) > 0)
			process_command_line(line, msh);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_context	*msh;
	t_bld_in	*builtins;
	int			ret_exit;

	(void)argc;
	(void)argv;
	msh = init_context(envp);
	check_shlvl(msh);
	builtins = create_builtin_list();
	msh->builtins = builtins;
	shell_loop(msh);
	ret_exit = msh->ret_exit;
	free_builtin_list(&builtins);
	cleanup_context(msh);
	clear_history();
	return (ret_exit);
}
