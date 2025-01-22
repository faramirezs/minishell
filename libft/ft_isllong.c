#include "libft.h"

#include <limits.h>
#include <ctype.h>
#include <stddef.h>

int ft_isllong(const char *str)
{
	long long num = 0;
	int sign = 1;
	const char *ptr = str;

	// Skip leading whitespace
	while (isspace(*ptr))
		ptr++;

	// Check for optional '+' or '-'
	if (*ptr == '+' || *ptr == '-')
	{
		if (*ptr == '-')
			sign = -1;
		ptr++;
	}

	// Check if there are no digits
	if (!isdigit(*ptr))
		return (0);

	// Convert string to long long while checking for overflow
	while (*ptr)
	{
		if (!isdigit(*ptr))
			return (0); // Invalid character

		int digit = *ptr - '0';

		if (sign == 1 && (num > (LLONG_MAX - digit) / 10))
			return (0); // Overflow for positive numbers
		if (sign == -1 && (num > (-(LLONG_MIN + digit)) / 10))
			return (0); // Overflow for negative numbers

		num = num * 10 + digit;
		ptr++;
	}

	return (1); // Valid `long long`
}
