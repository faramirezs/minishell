/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 14:08:17 by alramire          #+#    #+#             */
/*   Updated: 2024/05/06 11:53:54 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*uc_s;
	unsigned char	uc_c;

	uc_s = (unsigned char *)s;
	uc_c = (unsigned char)c;
	i = 0;
	while (i < n)
	{
		if (uc_s[i] == uc_c)
			return ((void *)&uc_s[i]);
		i++;
	}
	return (NULL);
}
