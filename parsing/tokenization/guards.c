
#include "../../headers/minishell.h"

void* OOM_GUARD(void *ptr, char *file, int number) //functions defined in header files should be marked as `inline` or moved to a separate source file to avoid multiple definitions during linking.
{
	if (ptr == NULL) {
		fprintf(stderr, "%s:%d Out of Memory", file, number);
		exit(EXIT_FAILURE);
	}
	return (ptr);
}
