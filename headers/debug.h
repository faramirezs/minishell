#ifndef DEBUG_H
# define DEBUG_H

# include <stdlib.h>
# include <string.h>
# include <stdio.h>

typedef struct s_alloc_info {
    void *ptr;
    int id;
    const char *file;
    int line;
    size_t size;
    const char *content;
} t_alloc_info;

# define MAX_ALLOCS 1000

void *debug_malloc(size_t size, const char *file, int line);
char *debug_strdup(const char *s, const char *file, int line);
void track_free(void *ptr, const char *file, int line);
void print_memory_stats(void);

extern int g_malloc_count;
extern t_alloc_info g_allocs[];

# ifndef NO_DEBUG
#  define malloc(size) debug_malloc(size, __FILE__, __LINE__)
#  define strdup(s) debug_strdup(s, __FILE__, __LINE__)
#  define free(ptr) track_free(ptr, __FILE__, __LINE__)
# endif

#endif