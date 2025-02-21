/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   guards.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 16:30:26 by alramire          #+#    #+#             */
/*   Updated: 2025/02/15 16:48:06 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

void *oom_guard(void *ptr, const char *file, int line)
{
    void *tracked_ptr;

    if (!ptr)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    
    // Track the allocation with debug_malloc
    tracked_ptr = debug_malloc(sizeof(ptr), file, line);
    if (!tracked_ptr)
    {
        perror("debug_malloc");
        exit(EXIT_FAILURE);
    }
    memcpy(tracked_ptr, ptr, sizeof(ptr));
    free(ptr);  // Free the original untracked allocation
    
    printf("\033[33mDEBUG: oom_guard tracking allocation at %p from %s:%d\033[0m\n", 
           tracked_ptr, file, line);
    
    return tracked_ptr;
}