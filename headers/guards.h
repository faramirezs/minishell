/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   guards.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 17:11:32 by alramire          #+#    #+#             */
/*   Updated: 2025/02/15 17:12:01 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GUARDS_H
# define GUARDS_H

# include "minishell.h"

// void	*oom_guard(void *ptr, char *file, int number);

void *oom_guard(void *ptr, const char *file, int line);

#endif
