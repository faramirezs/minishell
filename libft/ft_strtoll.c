#include "libft.h"
#include <limits.h>

static int	ft_isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

long long	ft_strtoll(const char *str, char **endptr, int base)
{
	long long   result;
	int         sign;
	const char  *s;
    
	result = 0;
	sign = 1;
	s = str;
	while (ft_isspace(*s))
		s++;
	if (*s == '-' || *s == '+')
	{
		sign = (*s == '-') ? -1 : 1;
		s++;
	}
	while (ft_isdigit(*s))
	{
		if (result > (LLONG_MAX - (*s - '0')) / 10)
		{
			result = (sign == 1) ? LLONG_MAX : LLONG_MIN;
			break ;
		}
		result = result * 10 + (*s - '0');
		s++;
	}
	if (endptr)
		*endptr = (char *)s;
	return (result * sign);
}