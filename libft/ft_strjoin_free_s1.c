#include "libft.h"

char *ft_strjoin_free_s1(char *s1, const char *s2)
{
    char *new_str;
    size_t len1 = 0;
    size_t len2 = 0;

    if (s1)
        len1 = strlen(s1);
    if (s2)
        len2 = strlen(s2);
    new_str = malloc(len1 + len2 + 1);
    if (!new_str)
        return NULL;
    if (s1)
        strcpy(new_str, s1);
    if (s2)
        strcpy(new_str + len1, s2);
    free(s1);
    return new_str;
}
