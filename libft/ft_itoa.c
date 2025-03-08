/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 15:00:09 by alramire          #+#    #+#             */
/*   Updated: 2024/05/06 11:54:20 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_n_size(long long n)
{
	int	len;

	len = 0;
	if (n < 1)
	{
		len++;
	}
	while (n)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

static char	*ft_tmp(int n)
{
	char	*str;

	str = (char *)malloc(sizeof(char) * (n + 1));
	if (!str)
		return (NULL);
	return (str);
}

char	*ft_itoa(int n)
{
	char		*s;
	int			len;
	long long	num;

	num = (long long)n;
	len = ft_n_size(n);
	s = ft_tmp(len);
	if (!s)
		return (NULL);
	if (n < 0)
	{
		num = -num;
		s[0] = 45;
	}
	s[len] = '\0';
	if (num == 0)
		s[0] = '0';
	while (num != 0)
	{
		s[len - 1] = ((num % 10) + '0');
		num = num / 10;
		len--;
	}
	return (s);
}
