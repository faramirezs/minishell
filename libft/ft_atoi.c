/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 18:04:09 by alramire          #+#    #+#             */
/*   Updated: 2024/04/29 10:54:32 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isspace(int c)
{
	if (c == 32 || (c >= 9 && c <= 13))
		return (1);
	return (0);
}

int	ft_atoi(const char *str)
{
	int	i;
	int	s;
	int	n;

	i = 0;
	s = 1;
	n = 0;
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '+' && str[i + 1] != '-')
		i++;
	if (str[i] == 45)
	{
		s = s * -1;
		i++;
	}
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		n = n * 10 + (str[i] - '0');
		i++;
	}
	return (n * s);
}

/* int main()
{

    // Test Case 1: Convert a basic positive number
    char *str1 = "42";
    if (ft_atoi(str1) == 42) {
        printf("\033[0;32m[PASS]\033[0m Correct conversion for '42'\n");
    } else {
        printf("\033[0;31m[FAIL]\033[0m Expected 42, got %d\n", ft_atoi(str1));
    }

} */
