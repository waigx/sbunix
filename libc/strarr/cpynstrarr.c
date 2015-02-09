/*
 *
 *  This file is part of SBUsh.
 *        SBUsh is a simple shell implementation, basically, it is
 *  an academic project of CSE506 of Stony Brook University in Spring 
 *  2015. For more details, please refer to README.md.
 *
 *  Copyright (C) 2015 Yigong Wang <yigwang@cs.stonybrook.edu>
 * 
 *
 *  SBUsh is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 * 
 *  SBUsh is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with SBUsh.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include <sys/defs.h>
#include <stdlib.h>
#include <string.h>


/**
 * cpynstrarr - copy n strings from an string array to another
 * @dest: Destination string array
 * @source: Source string array
 * @n: String number to be copied
 *
 * Returns dest
 *
 * The @dest should be allocated memories before use this function,
 * if the @source is shorter than @n, just copy all @source.
 * NOTE:
 *     * Unlike strncpy, this function will AUTOMATICALLY APPEND AN
 *       NULL POINTER to the end of @dest
 */
char **
cpynstrarr(char **dest, char **source, size_t n)
{
	char **dest_ptr = dest;

	while (n > 0 && *source != NULL) {
		*dest_ptr = malloc(strlen(*source) + 1);
		strcpy(*dest_ptr, *source);
		dest_ptr += 1;
		source += 1;
		n -= 1;
	}
	*dest_ptr = NULL;

	return dest;
}


