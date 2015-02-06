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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libcommon.h"


char **
splitstr(char *haystack, const char *needle)
{
	char **buf_ptr_ptr = malloc(sizeof(char *) * (strlen(haystack) + 2));
	char **current_buf_ptr = buf_ptr_ptr;
	char *last_ptr = haystack;
	char *current_ptr = haystack;
	size_t needle_len = strlen(needle);

	while (1) {
		current_ptr = strstr(last_ptr, needle);
		if (current_ptr == NULL) {
			*current_buf_ptr = malloc(strlen(last_ptr));
			strcpy(*current_buf_ptr, last_ptr);
			break;
		}
		*current_buf_ptr = malloc(current_ptr - last_ptr);
		strncpy(*current_buf_ptr, last_ptr, current_ptr - last_ptr);
		(*current_buf_ptr)[current_ptr - last_ptr] = '\0';
		last_ptr = current_ptr + needle_len;
		current_buf_ptr += 1;
	}
	current_buf_ptr += 1;
	*current_buf_ptr = NULL;

	return buf_ptr_ptr;

}


char *
freestrarr(char **buf_ptr_ptr, int isfreeall)
{
	char **ptr_ptr = buf_ptr_ptr;
	while (*ptr_ptr != NULL) {
		free(*ptr_ptr);
		ptr_ptr += 1;
	}
	if (isfreeall == 1) {
		free(buf_ptr_ptr);
	}
	return NULL;
}


char *
joinstrlst(char *buf, char **str_ptr, const char *salt)
{
	char *buf_ptr = buf;
	int salt_len = strlen(salt);

	if (str_ptr == NULL) {
		buf[0] = '\0';
		return buf;
	}

	while (*(str_ptr + 1) != NULL) {
		strcpy(buf_ptr, *str_ptr);
		buf_ptr += strlen(*str_ptr);
		strcpy(buf_ptr, salt);
		buf_ptr += salt_len;
		str_ptr += 1;
	}
	strcpy(buf_ptr, *str_ptr);

	return buf;
	
}


size_t
lenstrarr(char **ptr_ptr)
{
	size_t i = 0;
	
	while ( ptr_ptr[i] != 0 ) {
		i += 1;
	}

	return i;
}


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
