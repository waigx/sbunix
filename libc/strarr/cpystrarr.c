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
#include <string.h>
#include <stdlib.h>


/**
 * cpystrarr - copy a string array to new place
 * @dest: Destination string array
 * @source: Source string array
 *
 * Returns dest
 *
 * The @dest should be allocated memories before use this function
 */
char **
cpystrarr(char **dest, char **source)
{
	char **dest_ptr = dest;

	while (*source != NULL) {
		*dest_ptr = malloc(strlen(*source) + 1);
		strcpy(*dest_ptr, *source);
		dest_ptr += 1;
		source += 1;
	}
	*dest_ptr = NULL;
	
	return dest;
}
