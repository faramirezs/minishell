/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 16:59:21 by alramire          #+#    #+#             */
/*   Updated: 2025/02/16 16:21:53 by alramire         ###   ########.fr       */
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
