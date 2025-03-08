/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtoll.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mestefan <mestefan@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 15:29:43 by mestefan          #+#    #+#             */
/*   Updated: 2025/03/08 15:29:48 by mestefan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <limits.h>

static int	ft_isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

long long	check_overflow(long long result, int sign, char digit)
{
	if (result > (LLONG_MAX - (digit - '0')) / 10)
	{
		if (sign == 1)
			return (LLONG_MAX);
		else
			return (LLONG_MIN);
	}
	return (0);
}

void	check_sign(const char **s, int *sign)
{
	if (**s == '-' || **s == '+')
	{
		if (**s == '-')
			*sign = -1;
		else
			*sign = 1;
		(*s)++;
	}
}

long long	ft_strtoll(const char *str, char **endptr, int base)
{
	long long	result;
	int			sign;
	const char	*s;
	int			overflow;

	result = 0;
	sign = 1;
	s = str;
	while (ft_isspace(*s))
		s++;
	check_sign(&s, &sign);
	while (ft_isdigit(*s))
	{
		overflow = check_overflow(result, sign, *s);
		if (overflow)
		{
			result = overflow;
			break ;
		}
		result = result * base + (*s - '0');
		s++;
	}
	if (endptr)
		*endptr = (char *)s;
	return (result * sign);
}
