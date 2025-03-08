/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 01:20:29 by mestefan          #+#    #+#             */
/*   Updated: 2025/03/08 18:38:45 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/built_in.h"
#include "../../headers/minishell.h"

static int	ms_check_exit_arg(const char *arg, int *exit_code)
{
	int			i;
	char		*endptr;
	long long	num;

	i = 0;
	while (arg[i] == ' ')
		i++;
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	if (!isdigit(arg[i]))
	{
		*exit_code = 1;
		return (1);
	}
	num = ft_strtoll(arg, &endptr, 10);
	*exit_code = (int)(num % 256);
	if (*exit_code < 0)
		*exit_code += 256;
	return (0);
}

static void	cleanup_and_exit(struct s_tree_node *node, t_context *msh,
	int exit_code)
{
	printf("exit\n");
	free_tree_node(&node);
	free_builtin_list(&msh->builtins);
	cleanup_context(msh);
	clear_history();
	exit(exit_code);
}

static void	handle_no_args(struct s_tree_node *node, t_context *msh)
{
	msh->ret_exit = 0;
	cleanup_and_exit(node, msh, 0);
}

static void	handle_invalid_arg(struct s_tree_node *node,
	t_context *msh, char *arg)
{
	msh->ret_exit = 2;
	fprintf(stderr, "minishell: exit: %s: numeric argument required\n", arg);
	cleanup_and_exit(node, msh, 2);
}

int	handle_exit(struct s_tree_node *node, t_context *msh)
{
	int	exit_code;

	if (!node || !node->data.exec_u.args[1])
		handle_no_args(node, msh);
	if (ms_check_exit_arg(node->data.exec_u.args[1], &exit_code))
		handle_invalid_arg(node, msh, node->data.exec_u.args[1]);
	if (node->data.exec_u.args[2] != NULL)
	{
		fprintf(stderr, "minishell: exit: too many arguments\n");
		msh->ret_exit = 1;
		return (1);
	}
	msh->ret_exit = exit_code;
	cleanup_and_exit(node, msh, exit_code);
	return (0);
}
