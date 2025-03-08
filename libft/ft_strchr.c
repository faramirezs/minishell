/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 16:59:21 by alramire          #+#    #+#             */
/*   Updated: 2024/05/06 11:57:31 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char			char_c;
	unsigned int	i;

	char_c = (char)c;
	i = 0;
	while (s[i])
	{
		if (s[i] == char_c)
			return ((char *)&s[i]);
		i++;
	}
	if (s[i] == char_c)
		return ((char *)&s[i]);
	return (NULL);
}
/* 	After the loop, it checks one more time if the character
at index `i` is equal to `char_c`. This is to handle the case
where the character you're looking for is the null character
(`\0`). If it is, it returns a pointer to this character.
 */
