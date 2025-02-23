/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 01:20:29 by mestefan          #+#    #+#             */
/*   Updated: 2025/02/23 20:02:21 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/debug.h"
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
	num = strtoll(arg, &endptr, 10);
	*exit_code = (int)(num % 256);
	if (*exit_code < 0)
		*exit_code += 256;
	return (0);
}

int	handle_exit(struct s_tree_node *node, t_context *msh)
{
	int	exit_code;

	if (!node || !node->data.exec_u.args[1])
	{
		msh->ret_exit = 0;
		printf("exit\n");
		free_tree_node(node);
		exit(0);
	}
	if (ms_check_exit_arg(node->data.exec_u.args[1], &exit_code))
	{
		msh->ret_exit = 2;
		fprintf(stderr, "minishell: exit: %s: numeric argument required\n",
			node->data.exec_u.args[1]);
		free_tree_node(node);
		exit(2);
	}
	if (node->data.exec_u.args[2] != NULL)
	{
		fprintf(stderr, "minishell: exit: too many arguments\n");
		msh->ret_exit = 1;
		return (1);
	}
	msh->ret_exit = exit_code;
	printf("exit\n");
	print_memory_stats();
	free_tree_node(node);
	exit(exit_code);
}
