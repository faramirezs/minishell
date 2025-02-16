/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mestefan <mestefan@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 01:11:54 by mestefan          #+#    #+#             */
/*   Updated: 2025/02/16 01:11:58 by mestefan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s, size_t n)
{
	char	*copy;

	copy = malloc(sizeof(char) * (n + 1));
	if (copy == NULL)
		return (NULL);
	ft_memcpy(copy, s, n);
	copy[n] = '\0';
	return (copy);
}
