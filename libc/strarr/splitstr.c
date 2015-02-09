/*
 *
 *  This file is part of SBUsh
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
 * splitstr - split a string by given string
 * @haystack: the string to be splited
 * @needle: use this string to split @haystack
 *
 * Returns a pointer of char *, point to a string array
 *
 * REMEMBER TO FREE the returned result by freestrarr(char **, FREE_ALL)
 */
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
