#include "libft.h"

long ft_strtol(const char *str, char **endptr, int base)
{
    const char	*ptr;
    long		result;
    int			sign;
    int			digit;

    ptr = str;
	result = 0;
	sign = 1;
	// Skip leading whitespace
    while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' ||
           *ptr == '\v' || *ptr == '\f' || *ptr == '\r')
        ptr++;

    // Handle optional '+' or '-' sign
    if (*ptr == '+' || *ptr == '-')
    {
        if (*ptr == '-')
            sign = -1;
        ptr++;
    }

    // Auto-detect base if it's 0
    if (base == 0)
    {
        if (*ptr == '0')
        {
            if (*(ptr + 1) == 'x' || *(ptr + 1) == 'X')
            {
                base = 16; // Hexadecimal
                ptr += 2;
            }
            else
            {
                base = 8; // Octal
                ptr++;
            }
        }
        else
            base = 10; // Default to decimal
    }

    // Validate base
    if (base < 2 || base > 36)
    {
        if (endptr)
            *endptr = (char *)str;
        return 0;
    }

    // Parse the digits
    while (*ptr)
    {
        if (*ptr >= '0' && *ptr <= '9')
            digit = *ptr - '0';
        else if (*ptr >= 'a' && *ptr <= 'z')
            digit = *ptr - 'a' + 10;
        else if (*ptr >= 'A' && *ptr <= 'Z')
            digit = *ptr - 'A' + 10;
        else
            break; // Non-digit character found

        if (digit >= base)
            break ;

        // Check for overflow/underflow before multiplying
        if (result > (2147483647 - digit) / base)
            break ;

        result = result * base + digit;
        ptr++;
    }

    // Set endptr to point to the first unparsed character
    if (endptr)
        *endptr = (char *)ptr;

    // Handle overflow/underflow
    if (result > (2147483647 - digit) / base)
    {
        if (sign == 1)
            return (2147483647 - 1);
        else
            return (-2147483648 - 1);
    }
    
    return (sign * result);
}
