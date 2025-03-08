/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 17:49:10 by alramire          #+#    #+#             */
/*   Updated: 2024/04/26 16:50:29 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	char	*ptr;
	size_t	i;

	ptr = (char *)s;
	i = 0;
	while (i < n)
	{
		ptr[i++] = '\0';
	}
}

/* int main(void){
    char str[50] = "This is a test string";
    printf("Before memset: %s\n", str);
    ft_bzero(str, 5);
    printf("The new string is: %s\n", str);
    return 0;
} */
