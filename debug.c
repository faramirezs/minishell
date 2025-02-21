#include "headers/debug.h"
#include <stdio.h>

#undef malloc
#undef strdup
#undef free

int g_malloc_count = 0;
t_alloc_info g_allocs[MAX_ALLOCS] = {0};

static void record_allocation(void *ptr, int id, const char *file, int line, 
                            size_t size, const char *content)
{
    for (int i = 0; i < MAX_ALLOCS; i++)
    {
        if (g_allocs[i].ptr == NULL)
        {
            g_allocs[i].ptr = ptr;
            g_allocs[i].id = id;
            g_allocs[i].file = file;
            g_allocs[i].line = line;
            g_allocs[i].size = size;
            g_allocs[i].content = content;
            break;
        }
    }
}

static void remove_allocation(void *ptr)
{
    for (int i = 0; i < MAX_ALLOCS; i++)
    {
        if (g_allocs[i].ptr == ptr)
        {
            g_allocs[i].ptr = NULL;
            g_allocs[i].content = NULL;
            break;
        }
    }
}

const char *get_allocation_info(void *ptr)
{
    for (int i = 0; i < MAX_ALLOCS; i++)
    {
        if (g_allocs[i].ptr == ptr)
        {
            static char buf[256];
            snprintf(buf, sizeof(buf), "MALLOC #%d from %s:%d", 
                    g_allocs[i].id, g_allocs[i].file, g_allocs[i].line);
            return buf;
        }
    }
    return "Unknown allocation";
}

void *debug_malloc(size_t size, const char *file, int line)
{
    void *ptr = malloc(size);
    g_malloc_count++;
    printf("\033[33mMALLOC #%d: %p (%zu bytes) at %s:%d\033[0m\n", 
           g_malloc_count, ptr, size, file, line);
    record_allocation(ptr, g_malloc_count, file, line, size, NULL);
    return ptr;
}

char *debug_strdup(const char *s, const char *file, int line)
{
    char *ptr = strdup(s);
    g_malloc_count++;
    printf("\033[33mSTRDUP #%d: %p (%zu bytes) [%s] at %s:%d\033[0m\n", 
           g_malloc_count, ptr, strlen(s) + 1, s, file, line);
    record_allocation(ptr, g_malloc_count, file, line, strlen(s) + 1, s);
    return ptr;
}

void track_free(void *ptr, const char *file, int line)
{
    if (ptr)
    {
        for (int i = 0; i < MAX_ALLOCS; i++)
        {
            if (g_allocs[i].ptr == ptr)
            {
                printf("\033[33mFREE: %p at %s:%d (MALLOC #%d from %s:%d)\033[0m\n",
                       ptr, file, line, g_allocs[i].id, g_allocs[i].file, g_allocs[i].line);
                remove_allocation(ptr);
                free(ptr);
                g_malloc_count--;
                return;
            }
        }
        printf("\033[31mWARNING: Freeing untracked pointer %p at %s:%d\033[0m\n",
               ptr, file, line);
        free(ptr);
    }
}

void print_memory_stats(void)
{
    printf("\033[33m=== Memory Statistics ===\n");
    printf("Total allocations remaining: %d\n", g_malloc_count);
    if (g_malloc_count > 0)
    {
        printf("Remaining allocations:\n");
        for (int i = 0; i < MAX_ALLOCS; i++)
        {
            if (g_allocs[i].ptr)
            {
                printf("- %p: MALLOC #%d from %s:%d (%zu bytes)", 
                       g_allocs[i].ptr, g_allocs[i].id, 
                       g_allocs[i].file, g_allocs[i].line, 
                       g_allocs[i].size);
                if (g_allocs[i].content)
                    printf(" [%s]", g_allocs[i].content);
                printf("\n");
            }
        }
    }
    printf("=====================\033[0m\n");
}