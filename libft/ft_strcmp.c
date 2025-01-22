/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_strcmp.c										:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: jslusark <jslusark@student.42berlin.de>	+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/11/18 15:27:08 by jslusark		  #+#	#+#			 */
/*   Updated: 2024/11/18 15:27:35 by jslusark		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "libft.h"
int ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}