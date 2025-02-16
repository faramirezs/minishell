/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandroramirez <alejandroramirez@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 14:50:35 by alramire          #+#    #+#             */
/*   Updated: 2024/05/03 16:07:26 by alejandrora      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (needle[i] == 0)
		return ((char *) haystack);
	while (haystack[i] && i < len)
	{
		j = 0;
		while ((haystack[i + j] == needle[j] && haystack[i + j] && i + j < len))
		{
			if (needle[j] == 0)
				return ((char *) haystack + i);
			j++;
			if (j == ft_strlen(needle))
				return ((char *) haystack + i);
		}
		i++;
	}
	return (NULL);
}
