/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mestefan <mestefan@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 01:20:47 by mestefan          #+#    #+#             */
/*   Updated: 2025/02/10 01:20:50 by mestefan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/built_in.h"
#include "../../headers/minishell.h"

int	is_valid_key(const char *key)
{
	int	i;

	i = 0;
	if (!key || (!ft_isalpha(key[0]) && key[0] != '_'))
		return (0);
	while (key[i] != '\0')
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	set_environment_variable(char *key, char *value, t_context *msh)
{
	char	*new_var;

	new_var = ft_strjoin(key, "=");
	new_var = ft_strjoin_free_s1(new_var, value);
	ms_set_env(msh->env, msh, new_var);
	free(new_var);
	return (1);
}

int	process_key_value(char *arg, t_context *msh)
{
	char	*equal_sign;
	char	*key;
	char	*value;

	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		*equal_sign = '\0';
		key = arg;
		value = equal_sign + 1;
		if (!is_valid_key(key))
		{
			*equal_sign = '=';
			return (0);
		}
		set_environment_variable(key, value, msh);
		*equal_sign = '=';
		return (1);
	}
	return (is_valid_key(arg));
}

// int	process_key_value(char *arg, t_context *msh)
// {
// 	char	*equal_sign;
// 	char	*key;
// 	char	*value;
// 	char	*new_var;

// 	equal_sign = ft_strchr(arg, '=');
// 	if (equal_sign)
// 	{
// 		*equal_sign = '\0';
// 		key = arg;
// 		value = equal_sign + 1;
// 		if (!is_valid_key(key))
// 		{
// 			*equal_sign = '=';
// 			return (0);
// 		}
// 		new_var = ft_strjoin(key, "=");
// 		new_var = ft_strjoin_free_s1(new_var, value);
// 		ms_set_env(msh->env, msh, new_var);
// 		free(new_var);
// 		*equal_sign = '=';
// 		return (1);
// 	}
// 	else
// 	{
// 		if (is_valid_key(arg))
// 			return (1);
// 		return (0);
// 	}
// 	return (0);
// }

void	print_env_vars(t_context *msh, int in_pipe)
{
	int	i;
	int	output_fd;

	output_fd = STDOUT_FILENO;
	if (in_pipe)
		output_fd = msh->fd[1];
	i = 0;
	while (msh->env[i])
	{
		dprintf(output_fd, "declare -x %s\n", msh->env[i]);
		i++;
	}
}

int	handle_export(struct s_tree_node *node, t_context *msh)
{
	int		i;
	int		status;
	int		in_pipe;
	char	*arg;

	in_pipe = (msh->fd[0] != STDIN_FILENO || msh->fd[1] != STDOUT_FILENO);
	status = 0;
	if (!node->data.exec_u.args[1])
	{
		print_env_vars(msh, in_pipe);
		return (0);
	}
	i = 1;
	while (node->data.exec_u.args[i])
	{
		arg = node->data.exec_u.args[i++];
		if (!process_key_value(arg, msh))
		{
			fprintf(stderr, "export: `%s`: not a valid identifier\n", arg);
			status = 1;
		}
	}
	msh->ret_exit = status;
	return (status);
}
// int	handle_export(struct s_tree_node *node, t_context *msh)
// {
// 	int		i;
// 	char	*arg;
// 	int		in_pipe;
// 	int		status;

// 	in_pipe = (msh->fd[0] != STDIN_FILENO || msh->fd[1] != STDOUT_FILENO);
// 	status = 0;
// 	if (!node->data.exec_u.args[1])
// 	{
// 		i = 0;
// 		while (msh->env[i])
// 			printf("declare -x %s\n", msh->env[i++]);
// 		return (0);
// 	}
// 	i = 1;
// 	while (node->data.exec_u.args[i])
// 	{
// 		arg = node->data.exec_u.args[i];
// 		if (!process_key_value(arg, msh))
// 		{
// 			fprintf(stderr, "export: `%s`: not a valid identifier\n", arg);
// 			status = 1;
// 		}
// 		i++;
// 	}
// 	if (in_pipe)
// 	{
// 		i = 0;
// 		while (msh->env[i])
// 		{
// 			dprintf(msh->fd[1], "%s\n", msh->env[i]);
// 			i++;
// 		}
// 	}
// 	msh->ret_exit = status;
// 	return (status);
// }
