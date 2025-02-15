#include "libft.h"


char *ft_strndup(const char *s, size_t n)
{
	char *copy;

	copy = malloc(sizeof(char) * (n + 1));
	if (copy == NULL)
		return (NULL);
	ft_memcpy(copy, s, n);
	copy[n] = '\0';

	return (copy);
}


//size_t len = ft_strnlen(s, n); // Find the minimum of n and strlen(s)
//copy = oom_guardS(malloc(sizeof(char) * (n + 1)), __FILE__, __LINE__); 	// Allocate memory for the copy
// Null-terminate the string
// Copy up to `len` characters
