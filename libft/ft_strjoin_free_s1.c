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
        s1 ? s1 : "", s2 ? s2 : "");

    len1 = (s1) ? ft_strlen(s1) : 0;
    len2 = (s2) ? ft_strlen(s2) : 0;

    new_str = debug_malloc(len1 + len2 + 1, __FILE__, __LINE__);
    if (!new_str)
    {
        if (s1 && len1 > 0)  // Only free if non-empty
            track_free(s1, __FILE__, __LINE__);
        return (NULL);
    }

    if (s1 && len1 > 0)
        ft_memcpy(new_str, s1, len1);
    if (s2 && len2 > 0)
        ft_memcpy(new_str + len1, s2, len2);
    new_str[len1 + len2] = '\0';
    
    if (s1 && len1 > 0)  // Only free if non-empty
    {
        printf("\033[33mDEBUG: ft_strjoin_free_s1 freeing s1=[%s]\033[0m\n", s1);
        track_free(s1, __FILE__, __LINE__);
    }
    printf("\033[33mDEBUG: ft_strjoin_free_s1 returning [%s]\033[0m\n", new_str);
    
    return (new_str);
}