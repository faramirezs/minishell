#ifndef GUARDS_H
#define GUARDS_H

#include <stdio.h>
#include <stdlib.h>

void* OOM_GUARD(void *ptr, char *file, int number); //functions defined in header files should be marked as `inline` or moved to a separate source file to avoid multiple definitions during linking.

#endif
