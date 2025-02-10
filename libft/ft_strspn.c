#include "libft.h"

size_t ft_strspn(const char *str, const char *accept)
{
	size_t	  count;
	const char  *temp;

	count = 0;
	while (*str)
	{
		temp = accept;
		while (*temp)
		{
			if (*str == *temp)
			{
				count++;
				break ;
			}
			temp++;
		}
		if (!*temp) // No match found in accept
			break ;
		str++;
	}
	return (count);
}
