/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 13:24:53 by alramire          #+#    #+#             */
/*   Updated: 2024/05/06 11:56:52 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isascii(int c)
{
	return ((c >= 0) && (c <= 127));
}

/* int main()
{
	char c;
	char result;

	c = 'a';
	result = ft_isascii(c);
	printf("The result is for %d is %d\n", c, result);


	c = '2';
	result = ft_isascii(c);
	printf("The result is for %d is %d\n", c, result);


	c = '*';
	result = ft_isascii(c);
	printf("The result is for %d is %d\n", c, result);
	return (0);
} */
