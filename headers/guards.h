/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   guards.h										   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: alramire <alramire@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2025/01/10 17:10:02 by alramire		  #+#	#+#			 */
/*   Updated: 2025/01/10 17:10:04 by alramire		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#ifndef GUARDS_H
# define GUARDS_H

# include <stdio.h>
# include <stdlib.h>

void	*OOM_GUARD(void *ptr, char *file, int number);

#endif
