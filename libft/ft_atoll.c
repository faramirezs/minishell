/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoll.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 16:12:03 by alramire          #+#    #+#             */
/*   Updated: 2025/02/16 16:12:05 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long long ft_atoll(const char *str)
{
	long long num = 0;
	int sign = 1;

	// Skip leading whitespace
	while (isspace(*str))
		str++;

	// Check for optional '+' or '-'
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}

	// Convert digits to a number
	while (ft_isdigit(*str))
	{
		num = num * 10 + (*str - '0');
		str++;
	}

	return (num * sign);
}
