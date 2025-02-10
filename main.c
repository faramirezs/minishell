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

void	shell_loop(t_context *msh)
{
	char		*line;
	t_char_itr	itr;
	t_scanner	scanner;
	t_tree_node	*tree_node;

	while (1)
	{
		line = readline("minishell> ");
		if (!line || *line == '\0' || ft_strspn(line, " \t\n") == strlen(line))
		{
			free(line);
			continue ;
		}
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
