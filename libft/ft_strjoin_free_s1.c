/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_free_s1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mestefan <mestefan@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 01:11:25 by mestefan          #+#    #+#             */
/*   Updated: 2025/02/16 01:11:31 by mestefan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "stdio.h"
#include "../headers/debug.h"

char	*ft_strjoin_free_s1(char *s1, const char *s2)
{
	char	*new_str;
	size_t	len1;
	size_t	len2;

	printf("\033[33mDEBUG: ft_strjoin_free_s1 called with s1=[%s], s2=[%s]\033[0m\n", 
		s1 ? s1 : "NULL", s2 ? s2 : "NULL");

	if (!s1)
		len1 = 0;
	else
		len1 = ft_strlen(s1);
	if (!s2)
		len2 = 0;
	else
		len2 = ft_strlen(s2);
	new_str = debug_malloc(len1 + len2 + 1, __FILE__, __LINE__);
	if (!new_str)
	{
		free(s1);
		return (NULL);
	}
	if (s1)
		ft_memcpy(new_str, s1, len1);
	if (s2)
		ft_memcpy(new_str + len1, s2, len2);
	new_str[len1 + len2] = '\0';
	printf("\033[33mDEBUG: ft_strjoin_free_s1 freeing s1=[%s] and returning [%s]\033[0m\n", 
		s1 ? s1 : "NULL", new_str);
	free(s1);
	return (new_str);
}
